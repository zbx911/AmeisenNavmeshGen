# AmeisenNavmeshGen
Navmeshgenerator for WoW 3.3.5a 12340.

This is my attempt to create a navigation system using recast & detour for my WoW Bot.

## Feature checklist
**Essential:**
* ✔️ ADT parsing
* ✔️ Basic world geometry export
* ❌ M2/WMO geometry export
* ❌ Direct Navmesh export

**Optional:**
* ❌ MPQ reading

## How to use this
**1.** Export *World* folder from WoW's MPQ's using an MPQ editor or similar

**2.** Change the *input_directory* to the place where you exported the *World* folder
```c++
std::string input_directory = "F:\\WoW Mapping stuff";
```
Your structure should look like this:

"C:\\FOLDER_YOU_SELECTED\\World\\maps\\MAPNAME"

"C:\\FOLDER_YOU_SELECTED\\World\\wmo\\MAPNAME"

-> then your *input_directory* is "C:\\FOLDER_YOU_SELECTED"

**3.** Change the *export_directory* to a place where the \*.obj files will be dumped
```c++
std::string export_directory = "X:\NavmeshExport";
```

**4.** Pass filepaths into this array *sample_adts*
```c++
std::string maps_to_read[] = {
	"Azeroth"
};
```

**5.** Adjust the export dimension (you can export full map by starting at 1 and ending at 64 but do'nt try to open that map with recastDemo, it will crash)
```c++
constexpr int MAP_X_START = 32;
constexpr int MAP_X_END = 32;
constexpr int MAP_Y_START = 48;
constexpr int MAP_Y_END = 48;
```

**6.** Open the \*.obj with RecastDemo or do something else with them


## Credits

Without them this project would not exist.

❤️ wowdev wiki guys: https://wowdev.wiki/ADT/v18#ADT_files_and_blocks

❤️ Recast & Detour: https://github.com/recastnavigation/recastnavigation

## Screenshots

Some screenshots from the RecastDemo:

![alt text](https://github.com/Jnnshschl/AmeisenNavmeshGen/raw/master/images/sample_mesh_1.PNG "Azeroth_32_48.obj Sample mesh")

![alt text](https://github.com/Jnnshschl/AmeisenNavmeshGen/raw/master/images/sample_mesh_2.PNG "Sample path")
