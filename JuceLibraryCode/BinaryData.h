/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   env_rampdown_off_png;
    const int            env_rampdown_off_pngSize = 3641;

    extern const char*   env_rampdown_on_png;
    const int            env_rampdown_on_pngSize = 3554;

    extern const char*   env_rampup_off_png;
    const int            env_rampup_off_pngSize = 3615;

    extern const char*   env_rampup_on_png;
    const int            env_rampup_on_pngSize = 3550;

    extern const char*   env_random_off_png;
    const int            env_random_off_pngSize = 4038;

    extern const char*   env_random_on_png;
    const int            env_random_on_pngSize = 3991;

    extern const char*   env_rectangle_off_png;
    const int            env_rectangle_off_pngSize = 4027;

    extern const char*   env_rectangle_on_png;
    const int            env_rectangle_on_pngSize = 3955;

    extern const char*   env_sine_off_png;
    const int            env_sine_off_pngSize = 4397;

    extern const char*   env_sine_on_png;
    const int            env_sine_on_pngSize = 4167;

    extern const char*   env_triangle_off_png;
    const int            env_triangle_off_pngSize = 3326;

    extern const char*   env_triangle_on_png;
    const int            env_triangle_on_pngSize = 3288;

    extern const char*   inertia_hz_off_png;
    const int            inertia_hz_off_pngSize = 2778;

    extern const char*   inertia_hz_on_png;
    const int            inertia_hz_on_pngSize = 2701;

    extern const char*   inertia_note_off_png;
    const int            inertia_note_off_pngSize = 13315;

    extern const char*   inertia_note_on_png;
    const int            inertia_note_on_pngSize = 13230;

    extern const char*   inertia_off_off_png;
    const int            inertia_off_off_pngSize = 3116;

    extern const char*   inertia_off_on_png;
    const int            inertia_off_on_pngSize = 3070;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 18;

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
