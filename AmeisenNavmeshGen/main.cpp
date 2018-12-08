#include "main.h"
#include <windows.h>

// set this to false to avoid console spam
constexpr bool DEBUG = true;

// Set this values to the blocks that you want to export
// MIN 1
// MAX 64
constexpr int MAP_X_START = 32;
constexpr int MAP_X_END = 32;
constexpr int MAP_Y_START = 48;
constexpr int MAP_Y_END = 48;

int main(int argc, char* argv[]) {

	std::string input_directory = "F:\\WoW Mapping stuff";
	std::string export_directory = "C:\\Users\\Jannis\\Desktop\\recastnavigation\\RecastDemo\\Bin\\Meshes\\";

	// will be replaced with MPQ reader
	std::string maps_to_read[] = {
		"Azeroth"
	};

	// Create export directory
	CreateDirectory(export_directory.c_str(), NULL);
	int map_count = sizeof(maps_to_read) / sizeof(maps_to_read[0]);

	std::cout << "#> Ameisen Navmesh Generator (v" << VERSION << ")\n";
	std::cout << "#> ADT count: " << map_count << "\n";

	for (size_t m = 0; m < map_count; m++)
	{
		std::fstream obj_stream;
		std::stringstream vertex_buffer;
		std::stringstream index_buffer;

		int vertex_count = 1;

		// to prevent th E
		obj_stream << std::fixed << std::showpoint;
		obj_stream << std::setprecision(8);

		// output OBJ file stream, will use *.obj to test in the recast demo
		obj_stream.open(export_directory + "\\" + maps_to_read[m] + ".obj", std::fstream::out);

		for (size_t map_x = MAP_X_START; map_x <= MAP_X_END; map_x++)
		{
			for (size_t map_y = MAP_Y_START; map_y <= MAP_Y_END; map_y++)
			{
				std::stringstream filepath;
				filepath << input_directory << "\\World\\maps\\" << maps_to_read[m] << "\\" << maps_to_read[m] << "_" << map_x << "_" << map_y << ".adt";

				// check for file existence, not all 64x64 blocks have to be filled
				if (0xFFFFFFFF == GetFileAttributes(filepath.str().c_str())) { continue; }

				// Split "Azeroth_32_48" into "Azeroth", 32, 48 to parse these values later-----------------------------------------------
				int count = 0;
				size_t pos = 0;

				std::string token;
				std::string filename_values[3];

				std::string filename = filepath.str().substr(filepath.str().find_last_of("/\\") + 1); // remove folder path
				filename = filename.substr(0, filename.find_last_of('.')); // remove file extension

				while ((pos = filename.find("_")) != std::string::npos) {
					token = filename.substr(0, pos);
					filename_values[count] = token;
					filename.erase(0, pos + 1);
					count++;
				}
				filename_values[count] = filename;
				// End Split -------------------------------------------------------------------------------------------------------------

				std::cout << "#-> [" << m
					<< "] Processing: " << filename_values[0].c_str()
					<< " X: " << filename_values[1].c_str()
					<< " Y: " << filename_values[2].c_str() << "\n";


				// ########################################################################
				// Here starts the reading of information from our given ADT file/s
				// ########################################################################
				// Basic Map Geometry stuff-----------------------------------------------------------------------------------------------
				std::ifstream adt_stream;
				adt_stream.open(filepath.str(), std::fstream::in | std::fstream::binary);

				MVER adt_mver;
				MHDR adt_mhdr;
				MCIN adt_mcin;

				std::vector<MCIN_chunk> adt_mcin_chunks(16 * 16);
				std::vector<MCNK> adt_mcnks(16 * 16);
				std::vector<MCVT> adt_mcvts(16 * 16);
				std::vector<MCNR> adt_mcnrs(16 * 16);

				read_MVER(adt_mver, adt_stream);
				read_MHDR(adt_mhdr, adt_stream);

				////// ----------------------------------------------------
				//// recalculate the Offsets from MHDR, we need them later on
				// For some reason we need to add 0x14 (20) to the position
				const uint32_t MCIN_POSITION = reinterpret_cast<uint32_t>(adt_mhdr.mcin) + STATIC_POINTER_OFFSET;
				const uint32_t MDDF_POSITION = reinterpret_cast<uint32_t>(adt_mhdr.mddf) + STATIC_POINTER_OFFSET;
				const uint32_t MMID_POSITION = reinterpret_cast<uint32_t>(adt_mhdr.mmid) + STATIC_POINTER_OFFSET;
				const uint32_t MMDX_POSITION = reinterpret_cast<uint32_t>(adt_mhdr.mmdx) + STATIC_POINTER_OFFSET;
				const uint32_t MWMO_POSITION = reinterpret_cast<uint32_t>(adt_mhdr.mwmo) + STATIC_POINTER_OFFSET;
				const uint32_t MODF_POSITION = reinterpret_cast<uint32_t>(adt_mhdr.modf) + STATIC_POINTER_OFFSET;
				const uint32_t MWID_POSITION = reinterpret_cast<uint32_t>(adt_mhdr.mwid) + STATIC_POINTER_OFFSET;

				if (adt_stream.tellg() != MCIN_POSITION) { adt_stream.seekg(MCIN_POSITION); }

				read_MCIN_header(adt_mcin, adt_stream);
				read_MCIN_chunks(adt_mcin_chunks, adt_stream);
				read_MCNK(adt_mcnks, adt_mcin_chunks, adt_stream);
				read_MCVT(adt_mcvts, adt_mcnks, adt_mcin_chunks, adt_stream);
				read_MCNR(adt_mcnrs, adt_mcnks, adt_mcin_chunks, adt_stream);

				// Debug stuff
				output_parsing_debug_stuff(adt_mver, adt_mhdr, adt_mcin, adt_mcin_chunks, adt_mcnks, adt_mcvts, adt_mcnrs);
				// End Basic Map Geometry stuff-------------------------------------------------------------------------------------------

				// M2 stuff---------------------------------------------------------------------------------------------------------------
				MMDX adt_mmdx;
				MMID adt_mmid;
				MDDF_Header adt_mddf_header;

				std::vector<MDDF> adt_mddfs;
				std::vector<uint32_t> adt_mmids;
				std::vector<std::string> m2_paths;

				if (adt_stream.tellg() != MDDF_POSITION) { adt_stream.seekg(MDDF_POSITION); }
				read_MDDF(adt_mddf_header, adt_mddfs, adt_stream);

				if (adt_stream.tellg() != MMID_POSITION) { adt_stream.seekg(MMID_POSITION); }
				read_MMID(adt_mmid, adt_mmids, adt_stream);

				if (adt_stream.tellg() != MMDX_POSITION) { adt_stream.seekg(MMDX_POSITION); }
				read_MMDX(adt_mmdx, m2_paths, adt_stream);
				// End M2 stuff-----------------------------------------------------------------------------------------------------------

				// Sample M2 merging------------------------------------------------------------------------------------------------------
				if (DEBUG) {
					std::cout << "--> Sample Output of all M2 used in this ADT\n";
					for (int i = 0; i < adt_mddfs.size(); i++)
					{
						std::cout << "---> MDDF: M2 Path: " << m2_paths[adt_mddfs[i].name_id] << " Position: X: " << adt_mddfs[i].position.x << " Y: " << adt_mddfs[i].position.y << " Z: " << adt_mddfs[i].position.z << "\n";
					}
					std::cout << "#------------------------------------------------------------------------\n";
				}
				// End Sample M2 merging--------------------------------------------------------------------------------------------------

				// WMO stuff--------------------------------------------------------------------------------------------------------------
				MWMO adt_mwmo;
				MWID adt_mwid;
				MODF_Header adt_modf_header;

				std::vector<MODF> adt_modfs;
				std::vector<uint32_t> adt_mwids;
				std::vector<std::string> wmo_paths;

				if (adt_stream.tellg() != MODF_POSITION) { adt_stream.seekg(MODF_POSITION); }
				read_MODF(adt_modf_header, adt_modfs, adt_stream);

				if (adt_stream.tellg() != MWID_POSITION) { adt_stream.seekg(MWID_POSITION); }
				read_MWID(adt_mwid, adt_mwids, adt_stream);

				if (adt_stream.tellg() != MWMO_POSITION) { adt_stream.seekg(MWMO_POSITION); }
				read_MWMO(adt_mwmo, wmo_paths, adt_stream);
				// End WMO stuff----------------------------------------------------------------------------------------------------------

				// Sample WMO merging-----------------------------------------------------------------------------------------------------
				if (DEBUG) {
					std::cout << "--> Sample Output of all WMO used in this ADT\n";
					for (int i = 0; i < adt_modfs.size(); i++)
					{
						std::cout << "---> MDDF: WMO Path: " << wmo_paths[adt_modfs[i].name_id] << " Position: X: " << adt_modfs[i].position.x << " Y: " << adt_modfs[i].position.y << " Z: " << adt_modfs[i].position.z << "\n";
					}
					std::cout << "#------------------------------------------------------------------------\n";
				}
				// End Sample WMO merging-------------------------------------------------------------------------------------------------

				adt_stream.close();

				// ########################################################################
				// Here starts the processing of information we just read from the adt file
				// ########################################################################
				int chunk_count = 0;

				const float BLOCK_BASE_X = map_x * (BLOCK_SIZE);
				const float BLOCK_BASE_Y = map_y * (BLOCK_SIZE);
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
									float posX = (BLOCK_BASE_X)+(x * TILE_STEP8) + (j*(CHUNK_SIZE)); // adt_mcnks[chunk_count].position.x;
									float posY = (BLOCK_BASE_Y)+(y / 2 * TILE_STEP8) + (i*(CHUNK_SIZE)); // adt_mcnks[chunk_count].position.y;
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
									float posX = (BLOCK_BASE_X)+(x * TILE_STEP9) + (j*(CHUNK_SIZE)); // adt_mcnks[chunk_count].position.x;
									float posY = (BLOCK_BASE_Y)+(y / 2 * TILE_STEP9) + (i*(CHUNK_SIZE)); // adt_mcnks[chunk_count].position.y;
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
			}
		}

		obj_stream << vertex_buffer.str();
		obj_stream << index_buffer.str();
		obj_stream.close();
	}

	std::cin.get();
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

////// ----------------------------------------------------
//// Read the MMDX's
// #-- MMDX -> Map Map Doodads
// These things contain the DDX's used in this adt
void read_MMDX(MMDX &adt_mmdx, std::vector<std::string> &m2_paths, std::ifstream &adt_stream) {
	adt_stream.read((char*)&adt_mmdx, sizeof(adt_mmdx));
	m2_paths.reserve(adt_mmdx.size);

	std::stringstream ss;
	char c;

	for (int i = 0; i < adt_mmdx.size; i++)
	{
		adt_stream.read((char*)&c, sizeof(c));
		ss << c;

		if (c == 0x0) {
			m2_paths.push_back(ss.str());
			ss.str("");
		}
	}
}

////// ----------------------------------------------------
//// Read the MDDF's
// #-- MDDF -> Map Doodad M2 Placement
// These things contain the placement information for M2's
void read_MDDF(MDDF_Header &adt_mddf_header, std::vector<MDDF> &adt_mddfs, std::ifstream &adt_stream) {
	adt_stream.read((char*)&adt_mddf_header, sizeof(adt_mddf_header));
	for (int i = 0; i < adt_mddf_header.size / sizeof(MDDF); i++)
	{
		MDDF adt_mddf;
		adt_stream.read((char*)&adt_mddf, sizeof(adt_mddf));
		adt_mddfs.push_back(adt_mddf);
	}
}

////// ----------------------------------------------------
//// Read the MMID's
// #-- MMID -> Map M2 References
// These things contain the placement information for M2's
void read_MMID(MMID &adt_mmid, std::vector<uint32_t> &adt_mmids, std::ifstream &adt_stream) {
	adt_stream.read((char*)&adt_mmid, sizeof(adt_mmid));
	for (int i = 0; i < adt_mmid.size / sizeof(uint32_t); i++)
	{
		uint32_t offset;
		adt_stream.read((char*)&offset, sizeof(offset));
		adt_mmids.push_back(offset);
	}
}

////// ----------------------------------------------------
//// Read the MWMO's
// #-- MWMO -> Map World Map Objects
// These things contain the WMO's used in this adt
void read_MWMO(MWMO &adt_mwmo, std::vector<std::string> &wmo_paths, std::ifstream &adt_stream) {
	adt_stream.read((char*)&adt_mwmo, sizeof(adt_mwmo));
	wmo_paths.reserve(adt_mwmo.size);

	std::stringstream ss;
	char c;

	for (int i = 0; i < adt_mwmo.size; i++)
	{
		adt_stream.read((char*)&c, sizeof(c));
		ss << c;

		if (c == 0x0) {
			wmo_paths.push_back(ss.str());
			ss.str("");
		}
	}
}

void read_MODF(MODF_Header &adt_modf_header, std::vector<MODF> &adt_modfs, std::ifstream &adt_stream) {
	adt_stream.read((char*)&adt_modf_header, sizeof(adt_modf_header));
	for (int i = 0; i < adt_modf_header.size / sizeof(MODF); i++)
	{
		MODF adt_modf;
		adt_stream.read((char*)&adt_modf, sizeof(adt_modf));
		adt_modfs.push_back(adt_modf);
	}
}

void read_MWID(MWID &adt_mwid, std::vector<uint32_t> &adt_mwids, std::ifstream &adt_stream) {
	adt_stream.read((char*)&adt_mwid, sizeof(adt_mwid));
	for (int i = 0; i < adt_mwid.size / sizeof(uint32_t); i++)
	{
		uint32_t offset;
		adt_stream.read((char*)&offset, sizeof(offset));
		adt_mwids.push_back(offset);
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
		/*
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
		*/
	}
}