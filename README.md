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
**1.** Export ADT files using an MPQ editor or similar
**2.** Pass filepaths into this array *sample_adts*
```c++
std::string sample_adts[] = {
    "F:\\WoW Mapping stuff\\World\\maps\\Azeroth\\Azeroth_31_48.adt",
    "F:\\WoW Mapping stuff\\World\\maps\\Azeroth\\Azeroth_31_47.adt",
    "F:\\WoW Mapping stuff\\World\\maps\\Azeroth\\Azeroth_32_47.adt",
    "F:\\WoW Mapping stuff\\World\\maps\\Azeroth\\Azeroth_32_48.adt"
};
```
**3.** Change the *export_directory* to a place where the \*.obj files will be dumped
```c++
std::string export_directory = "X:\NavmeshExport";
```
**4.** Open the \*.obj with RecastDemo or do something else with them


## Credits

Without them this project would not exist.

❤️ wowdev wiki guys: https://wowdev.wiki/ADT/v18#ADT_files_and_blocks

❤️ Recast & Detour: https://github.com/recastnavigation/recastnavigation

## Screenshots

Some screenshots from the RecastDemo:

![alt text](https://github.com/Jnnshschl/AmeisenNavmeshGen/raw/master/images/sample_mesh_1.PNG "Azeroth_32_48.obj Sample mesh")

![alt text](https://github.com/Jnnshschl/AmeisenNavmeshGen/raw/master/images/sample_mesh_2.PNG "Sample path")
