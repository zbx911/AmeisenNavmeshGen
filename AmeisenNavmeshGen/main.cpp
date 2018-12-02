#include "main.h"
#include <windows.h>

// set this to false to avoid console spam
constexpr bool DEBUG = false;

int main(int argc, char* argv[]) {

	// will be replaced with MPQ reader
	std::string sample_adts[] = {
		"F:\\WoW Mapping stuff\\World\\maps\\Azeroth\\Azeroth_31_48.adt",
		"F:\\WoW Mapping stuff\\World\\maps\\Azeroth\\Azeroth_31_47.adt",
		"F:\\WoW Mapping stuff\\World\\maps\\Azeroth\\Azeroth_32_47.adt",
		"F:\\WoW Mapping stuff\\World\\maps\\Azeroth\\Azeroth_32_48.adt"
	};

	std::string export_directory = "X:\NavmeshExport";

	// Create export directory
	CreateDirectory(export_directory.c_str(), NULL);
	int adt_count = sizeof(sample_adts) / sizeof(sample_adts[0]);

	std::cout << "#> Ameisen Navmesh Generator (v" << VERSION << ")\n";
	std::cout << "#> ADT count: " << adt_count << "\n";

	for (size_t i = 0; i < adt_count; i++)
	{
		std::string filename = sample_adts[i].substr(sample_adts[i].find_last_of("/\\") + 1); // remove folder path
		filename = filename.substr(0, filename.find_last_of('.')); // remove file extension

		// output OBJ file stream, will use *.obj to test in the recast demo
		std::fstream obj_stream;
		obj_stream.open(export_directory + "\\" + filename + ".obj", std::fstream::out);

		// Split "Azeroth_32_48" into "Azeroth", 32, 48 to parse these values later
		int count = 0;
		size_t pos = 0;
		std::string token;
		std::string filename_values[3];

		while ((pos = filename.find("_")) != std::string::npos) {
			token = filename.substr(0, pos);
			filename_values[count] = token;
			filename.erase(0, pos + 1);
			count++;
		}
		filename_values[count] = filename;

		std::cout << "#-> [" << i
			<< "] Processing: " << filename_values[0].c_str()
			<< " X: " << filename_values[1].c_str()
			<< " Y: " << filename_values[2].c_str() << "\n";

		// Reading part
		std::ifstream adt_stream;
		adt_stream.open(sample_adts[i].c_str(), std::fstream::in | std::fstream::binary);

		MVER adt_mver;
		read_MVER(adt_mver, adt_stream);

		MHDR adt_mhdr;
		read_MHDR(adt_mhdr, adt_stream);

		////// ----------------------------------------------------
		//// Check if the we are at the right position to read MCIN
		// For some reason we need to add 0x14 (20) to the position
		const uint32_t MCIN_POSITION = reinterpret_cast<uint32_t>(adt_mhdr.mcin) + STATIC_POINTER_OFFSET;

		if (adt_stream.tellg() != MCIN_POSITION) {
			adt_stream.seekg(MCIN_POSITION);
		}

		MCIN adt_mcin;
		read_MCIN_header(adt_mcin, adt_stream);

		std::vector<MCIN_chunk> adt_mcin_chunks(16 * 16);
		read_MCIN_chunks(adt_mcin_chunks, adt_stream);

		std::vector<MCNK> adt_mcnks(16 * 16);
		read_MCNK(adt_mcnks, adt_mcin_chunks, adt_stream);

		std::vector<MCVT> adt_mcvts(16 * 16);
		read_MCVT(adt_mcvts, adt_mcnks, adt_mcin_chunks, adt_stream);

		std::vector<MCNR> adt_mcnrs(16 * 16);
		read_MCNR(adt_mcnrs, adt_mcnks, adt_mcin_chunks, adt_stream);

		adt_stream.close();
		output_parsing_debug_stuff(adt_mver, adt_mhdr, adt_mcin, adt_mcin_chunks, adt_mcnks, adt_mcvts, adt_mcnrs);

		// to prevent th E
		obj_stream << std::fixed << std::showpoint;
		obj_stream << std::setprecision(8);

		std::stringstream vertex_buffer;
		std::stringstream index_buffer;
		int chunk_count = 0;
		int vertex_count = 1;

		int block_x = atoi(filename_values[1].c_str());
		int block_y = atoi(filename_values[2].c_str());

		const float BLOCK_BASE_X = block_x * BLOCK_SIZE;
		const float BLOCK_BASE_Y = block_y * BLOCK_SIZE;
		// OBJ output
		for (size_t i = 0; i < 16; i++)
		{
			for (size_t j = 0; j < 16; j++)
			{
				int vertex_index = 0;
				for (size_t y = 0; y < 9 + 8; y++)
				{
					if ((y + 1) % 2 == 0) // We are in the 8 items row
					{
						for (size_t x = 0; x < 8; x++)
						{
							float posX = (BLOCK_BASE_X / 2) + (x * TILE_STEP8) + (j*(CHUNK_SIZE - 3.7F)); // adt_mcnks[chunk_count].position.x;
							float posY = (BLOCK_BASE_Y / 2) + (y / 2 * TILE_STEP8) + (i*(CHUNK_SIZE - 3.7F)); // adt_mcnks[chunk_count].position.y;
							float posZ = adt_mcnks[chunk_count].position.z + adt_mcvts[chunk_count].heights[vertex_index];

							vertex_buffer << "v " << -posY << " " << posZ << " " << -posX << "\n";

							index_buffer << "f " << vertex_count << " " << vertex_count - 9 << " " << vertex_count - 8 << "\n";
							index_buffer << "f " << vertex_count << " " << vertex_count + 9 << " " << vertex_count + 8 << "\n";
							index_buffer << "f " << vertex_count << " " << vertex_count + 8 << " " << vertex_count - 9 << "\n";
							index_buffer << "f " << vertex_count << " " << vertex_count - 8 << " " << vertex_count + 9 << "\n";
							index_buffer << "f " << vertex_count << " " << vertex_count - 8 << " " << vertex_count + 9 << "\n";

							vertex_count++;
							vertex_index++;
						}
					}
					else // We are in the 9 items row
					{
						for (size_t x = 0; x < 9; x++)
						{
							float posX = (BLOCK_BASE_X / 2) + (x * TILE_STEP9) + (j*(CHUNK_SIZE - 3.7F)); // adt_mcnks[chunk_count].position.x;
							float posY = (BLOCK_BASE_Y / 2) + (y / 2 * TILE_STEP9) + (i*(CHUNK_SIZE - 3.7F)); // adt_mcnks[chunk_count].position.y;
							float posZ = adt_mcnks[chunk_count].position.z + adt_mcvts[chunk_count].heights[vertex_index];

							vertex_buffer << "v " << -posY << " " << posZ << " " << -posX << "\n";

							vertex_count++;
							vertex_index++;
						}
					}
				}
				chunk_count++;
			}
		}

		obj_stream << vertex_buffer.str();
		obj_stream << index_buffer.str();
		obj_stream.close();
	}


	return 0;
}

////// ----------------------------------------------------
//// Read the MVER
// #-- MVER -> Map Version
// Read the ADT version and this reversed bs descriptor...
//
// Note to myself: Just in case you wonder why the desc value is some weird trash, read it backwards :^)
void read_MVER(MVER &adt_mver, std::ifstream &adt_stream) {
	adt_stream.read((char*)&adt_mver, sizeof(adt_mver));
}

////// ----------------------------------------------------
//// Read the MHDR
// #-- MHDR -> Map Header
// Contains 10 Pointers to other Map related information and two flags
// Flags:
// The first flags indicates wether there is a MFBO inside or not
// The second is some sort of trash set sometimes on Northrend
void read_MHDR(MHDR &adt_mhdr, std::ifstream &adt_stream) {
	adt_stream.read((char*)&adt_mhdr, sizeof(adt_mhdr));
}

////// ----------------------------------------------------
//// Read the MCIN Header
// #-- MCIN -> Map Chunk Info
// The MCIN header, it's useless to us
void read_MCIN_header(MCIN &adt_mcin, std::ifstream &adt_stream) {
	adt_stream.read((char*)&adt_mcin, sizeof(adt_mcin));
}

////// ----------------------------------------------------
//// Read the MCIN_chunks
// #-- MCIN_chunk -> Map Chunk Info
// These chunks contain the offsets to MCNK's
// There are a total of 256 chunks in here (16 * 16)
void read_MCIN_chunks(std::vector<MCIN_chunk> &adt_mcin_chunks, std::ifstream &adt_stream) {
	adt_mcin_chunks.reserve(16 * 16);
	for (size_t i = 0; i < 16 * 16; i++)
	{
		MCIN_chunk chunk;
		adt_stream.read((char*)&chunk, sizeof(chunk));
		adt_mcin_chunks[i] = chunk;
	}
}

////// ----------------------------------------------------
//// Read the MCNK's
// #-- MCNK -> Map Chunk
// These chunks contain the offsets to MCNK's
// There are a total of 256 chunks in here (16 * 16)
// 
// Note to myself: coordinates are in hex because it's
//				   base16 and makes sense if you have
//                 a 16 * 16 field
void read_MCNK(std::vector<MCNK> &adt_mcnks, std::vector<MCIN_chunk> &adt_mcin_chunks, std::ifstream &adt_stream) {
	adt_mcnks.reserve(16 * 16);
	for (size_t i = 0; i < 16 * 16; i++)
	{
		// Move the stream to read at the MCNK offset
		adt_stream.seekg(adt_mcin_chunks[i].offset);

		MCNK mcnk;
		adt_stream.read((char*)&mcnk, sizeof(mcnk));
		adt_mcnks[i] = mcnk;
	}
}

////// ----------------------------------------------------
//// Read the MCVT's
// #-- MCVT -> Map Vertexes
// These things contain the height values
void read_MCVT(std::vector<MCVT> &adt_mcvt, std::vector<MCNK> &adt_mcnks, std::vector<MCIN_chunk> &adt_mcin_chunks, std::ifstream &adt_stream) {
	adt_mcvt.reserve(16 * 16);
	for (size_t i = 0; i < 16 * 16; i++)
	{
		// Move the stream to read at the MCVT offset
		adt_stream.seekg(adt_mcin_chunks[i].offset + adt_mcnks[i].offset_mcvt);

		MCVT mcvt;
		adt_stream.read((char*)&mcvt, sizeof(mcvt));
		adt_mcvt[i] = mcvt;
	}
}

////// ----------------------------------------------------
//// Read the MCNR's
// #-- MCNR -> Map Normals
// These things contain the normals
void read_MCNR(std::vector<MCNR> &adt_mcnr, std::vector<MCNK> &adt_mcnks, std::vector<MCIN_chunk> &adt_mcin_chunks, std::ifstream &adt_stream) {
	adt_mcnr.reserve(16 * 16);
	for (size_t i = 0; i < 16 * 16; i++)
	{
		// Move the stream to read at the MCVT offset
		adt_stream.seekg(adt_mcin_chunks[i].offset + adt_mcnks[i].offset_mcnr);

		MCNR mcnr;
		adt_stream.read((char*)&mcnr, sizeof(mcnr));
		adt_mcnr[i] = mcnr;
	}
}

void output_parsing_debug_stuff(MVER adt_mver, MHDR adt_mhdr, MCIN adt_mcin, std::vector<MCIN_chunk> adt_mcin_chunks, std::vector<MCNK> adt_mcnks, std::vector<MCVT> adt_mcvts, std::vector<MCNR> adt_mcnrs) {
	if (DEBUG) {
		std::cout << "#--> ADT Size: " << adt_mver.size << " Version: " << adt_mver.version << " Desc: \"" << adt_mver.desc << "\"\n";
		std::cout << "#------------------------------------------------------------------------\n";
		std::cout << "#---> Flags: " << adt_mhdr.flags << "\n";
		std::cout << "#---> MCIN*: 0x" << adt_mhdr.mcin << "\n";
		std::cout << "#---> MDDF*: 0x" << adt_mhdr.mddf << "\n";
		std::cout << "#---> MFBO*: 0x" << adt_mhdr.mfbo << "\n";
		std::cout << "#---> MH2O*: 0x" << adt_mhdr.mh2o << "\n";
		std::cout << "#---> MMDX*: 0x" << adt_mhdr.mmdx << "\n";
		std::cout << "#---> MMID*: 0x" << adt_mhdr.mmid << "\n";
		std::cout << "#---> MODF*: 0x" << adt_mhdr.modf << "\n";
		std::cout << "#---> MTEX*: 0x" << adt_mhdr.mtex << "\n";
		std::cout << "#---> MTXF*: 0x" << adt_mhdr.mtxf << "\n";
		std::cout << "#---> MWID*: 0x" << adt_mhdr.mwid << "\n";
		std::cout << "#---> MWMO*: 0x" << adt_mhdr.mwmo << "\n";
		std::cout << "#------------------------------------------------------------------------\n";
		std::cout << "#--> MCIN Size: " << adt_mcin.size << " Desc: \"" << adt_mcin.desc << "\"\n";
		std::cout << "#------------------------------------------------------------------------\n";
		for (size_t i = 0; i < adt_mcin_chunks.size(); i++)
		{
			std::cout << "#---> MCIN_chunk Size: 0x" << std::hex << adt_mcin_chunks[i].size << " Offset: 0x" << std::hex << adt_mcin_chunks[i].offset << "\n";
		}
		std::cout << "#------------------------------------------------------------------------\n";
		for (size_t i = 0; i < adt_mcnks.size(); i++)
		{
			std::cout << "#---> MCNK AreaId: 0x" << adt_mcnks[i].areaid << " X: 0x" << adt_mcnks[i].index_x << " Y: 0x" << adt_mcnks[i].index_y << "\n";
			std::cout << "      Position X:" << adt_mcnks[i].position.x << " Y: " << adt_mcnks[i].position.y << " Z: " << adt_mcnks[i].position.z << "\n";
			std::cout << "      MCVT Offset: 0x" << adt_mcnks[i].offset_mcvt << " MCNR Offset: 0x" << adt_mcnks[i].offset_mcnr << " MCLQ Offset: 0x" << adt_mcnks[i].offset_mclq << "\n";
		}
		std::cout << "#------------------------------------------------------------------------\n";
		for (size_t i = 0; i < adt_mcvts.size(); i++)
		{
			std::cout << "#---> MCNVT Desc: " << adt_mcvts[i].desc << " Size: " << adt_mcvts[i].size << " First Height: " << adt_mcvts[i].heights[0] << "\n";
		}
		std::cout << "#------------------------------------------------------------------------\n";
		for (size_t i = 0; i < adt_mcnrs.size(); i++)
		{
			std::cout << "#---> MCNR Desc: " << adt_mcnrs[i].desc << " Size: " << adt_mcnrs[i].size << " First Normal: " << adt_mcnrs[i].normals[0] << "\n";
		}
		std::cout << "#------------------------------------------------------------------------\n";
	}
}