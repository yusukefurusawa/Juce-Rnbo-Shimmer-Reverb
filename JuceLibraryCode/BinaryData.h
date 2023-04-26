/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   AUInfo_plist_in;
    const int            AUInfo_plist_inSize = 1798;

    extern const char*   PkgInfo;
    const int            PkgInfoSize = 8;

    extern const char*   UnityInfo_plist_in;
    const int            UnityInfo_plist_inSize = 1016;

    extern const char*   VST3Info_plist_in;
    const int            VST3Info_plist_inSize = 1016;

    extern const char*   CCache_cmake;
    const int            CCache_cmakeSize = 158;

    extern const char*   conan_cmake;
    const int            conan_cmakeSize = 41319;

    extern const char*   MinGWSTDThreads_cmake;
    const int            MinGWSTDThreads_cmakeSize = 672;

    extern const char*   rnbo_description_h_in;
    const int            rnbo_description_h_inSize = 154;

    extern const char*   RNBOConan_cmake;
    const int            RNBOConan_cmakeSize = 479;

    extern const char*   RNBODescriptionHeader_cmake;
    const int            RNBODescriptionHeader_cmakeSize = 240;

    extern const char*   RNBOJuce_cmake;
    const int            RNBOJuce_cmakeSize = 603;

    extern const char*   SCCache_cmake;
    const int            SCCache_cmakeSize = 162;

    extern const char*   LICENSE_md;
    const int            LICENSE_mdSize = 1772;

    extern const char*   LICENSE_md2;
    const int            LICENSE_md2Size = 1614;

    extern const char*   LICENSE;
    const int            LICENSESize = 1056;

    extern const char*   version_cmake;
    const int            version_cmakeSize = 165;

    extern const char*   dependencies_json;
    const int            dependencies_jsonSize = 2;

    extern const char*   description_json;
    const int            description_jsonSize = 109143;

    extern const char*   presets_json;
    const int            presets_jsonSize = 22324;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 19;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
