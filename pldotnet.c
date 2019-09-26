#include "pldotnet.h"

//// DOTNET-HOST-SAMPLE STUFF ///////////////////////////////////////
// Provided by the AppHost NuGet package and installed as an SDK pack
#include <nethost.h>
// Header files copied from https://github.com/dotnet/core-setup
#include <coreclr_delegates.h>
#include <hostfxr.h>
#include <dlfcn.h>
#include <limits.h>
#define STR(s) s
#define CH(c) c
#define DIR_SEPARATOR '/'
#define MAX_PATH PATH_MAX
// Null pointer constant definition
#define nullptr ((void*)0)
// Globals to hold hostfxr exports
hostfxr_initialize_for_runtime_config_fn init_fptr;
hostfxr_get_runtime_delegate_fn get_delegate_fptr;
hostfxr_close_fn close_fptr;
// Forward declarations
int load_hostfxr();
load_assembly_and_get_function_pointer_fn get_dotnet_load_assembly(const char_t *assembly);
// TODO: Check how Postgres API do this or shoud we use C lib
#define SNPRINTF(name, size, fmt, ...)                  \
    char name[size];                                    \
    i = snprintf(name, sizeof(name), fmt, __VA_ARGS__); \
    if(i > sizeof(name)){                               \
        fprintf(stderr, "String too long: " #name);     \
        exit(-1);                                       \
    }
//// DOTNET-HOST-SAMPLE STUFF ///////////////////////////////////////

PG_MODULE_MAGIC;

// Declare extension variables/structs here

PGDLLEXPORT Datum _PG_init(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum _PG_fini(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum pldotnet_call_handler(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum pldotnet_validator(PG_FUNCTION_ARGS);
#if PG_VERSION_NUM >= 90000
PGDLLEXPORT Datum pldotnet_inline_handler(PG_FUNCTION_ARGS);
#endif

PG_FUNCTION_INFO_V1(_PG_init);
Datum _PG_init(PG_FUNCTION_ARGS) 
{
    // Initialize variable structs here 
    // Init dotnet runtime here ?


    PG_RETURN_VOID();
}

PG_FUNCTION_INFO_V1(_PG_fini);
Datum _PG_fini(PG_FUNCTION_ARGS) 
{
    // Deinitialize variable/structs here
    // Close dotnet runtime here ?
    PG_RETURN_VOID();
}

PG_FUNCTION_INFO_V1(pldotnet_call_handler);
Datum pldotnet_call_handler(PG_FUNCTION_ARGS) 
{
//    return DotNET_callhandler( /* additional args, */ fcinfo);
    Datum retval = 0;
    bool istrigger;
    if (SPI_connect() != SPI_OK_CONNECT)
        elog(ERROR, "[pldotnet]: could not connect to SPI manager");
    istrigger = CALLED_AS_TRIGGER(fcinfo);
    if (istrigger) {
        ereport(ERROR,
              (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
               errmsg("[pldotnet]: dotnet trigger not supported")));
    }
    // do dotnet initialization and checks
    // ...
    PG_TRY();
    {
        // dot dotnet magic
        // how to get args ? fcinfo
        //retval = //(dotnet result of computation)
    }
    PG_CATCH();
    {
        // Do the excption handling
        PG_RE_THROW();
    }
    PG_END_TRY();
    if (SPI_finish() != SPI_OK_FINISH)
        elog(ERROR, "[pldotnet]: could not disconnect from SPI manager");
    return retval; 
}

PG_FUNCTION_INFO_V1(pldotnet_validator);
Datum pldotnet_validator(PG_FUNCTION_ARGS) 
{
//    return DotNET_validator(/* additional args, */ PG_GETARG_OID(0));
    if (SPI_connect() != SPI_OK_CONNECT)
        elog(ERROR, "[pldotnet]: could not connect to SPI manager");
    PG_TRY();
    {
        // Do some dotnet checking ??
    }
    PG_CATCH();
    {
        // Do the excption handling
        PG_RE_THROW();
    }
    PG_END_TRY();
    if (SPI_finish() != SPI_OK_FINISH)
        elog(ERROR, "[pldotnet]: could not disconnect from SPI manager");
    return 0; /* VOID */
}

#if PG_VERSION_NUM >= 90000
#define CODEBLOCK \
  ((InlineCodeBlock *) DatumGetPointer(PG_GETARG_DATUM(0)))->source_text

PG_FUNCTION_INFO_V1(pldotnet_inline_handler);
Datum pldotnet_inline_handler(PG_FUNCTION_ARGS) 
{
//  return DotNET_inlinehandler( /* additional args, */ CODEBLOCK);
    if (SPI_connect() != SPI_OK_CONNECT)
        elog(ERROR, "[plldotnet]: could not connect to SPI manager");
    PG_TRY();
    {
        // Run dotnet anonymous here  CODEBLOCK has the inlined source code

    // Get the current executable's directory
    // This sample assumes the managed assembly to load and its runtime configuration file are next to the host
    int i;
    char_t host_path[MAX_PATH];

    char* resolved = realpath(argv[0], host_path);
    assert(resolved != nullptr);

//// DOTNET-HOST-SAMPLE STUFF ///////////////////////////////////////
    const char* source_code = CODEBLOCK;
    //
    // STEP 0: Compile C# source code
    //
    // char default_dnldir[] = "/DEBUG/home/app/src/DotNetLib/";
    char default_dnldir[] = "/home/app/src/DotNetLib/";
    char *dnldir = getenv("DNLDIR");
    if (dnldir == nullptr) dnldir = &default_dnldir[0];
    SNPRINTF(filename, 1024, "%s/Lib.cs", dnldir);

    FILE *output_file = fopen(filename, "w+");
    if (!output_file) {
        fprintf(stderr, "Cannot open file: '%s'\n", filename);
        exit(-1);
    }
    fputs(source_code, output_file);
    fclose(output_file);

    SNPRINTF(cmd, 1024, "dotnet build %s > nul", dnldir);
    int compile_resp = system(cmd);
    assert(compile_resp != -1 && "Failure: Cannot compile C# source code");

    char* root_path = strdup(host_path);
    *(rindex(root_path, DIR_SEPARATOR)+1) = 0;

    //
    // STEP 1: Load HostFxr and get exported hosting functions
    //
    if (!load_hostfxr()) assert(0 && "Failure: load_hostfxr()");

    //
    // STEP 2: Initialize and start the .NET Core runtime
    //
    SNPRINTF(config_path, 1024, "%s/DotNetLib.runtimeconfig.json", root_path);
    fprintf(stderr, "# DEBUG: config_path is '%s'.\n", config_path);

    load_assembly_and_get_function_pointer_fn load_assembly_and_get_function_pointer = nullptr;
    load_assembly_and_get_function_pointer = get_dotnet_load_assembly(config_path);
    assert(load_assembly_and_get_function_pointer != nullptr && \
        "Failure: get_dotnet_load_assembly()");

    //
    // STEP 3: Load managed assembly and get function pointer to a managed method
    //
    SNPRINTF(dotnetlib_path, 1024, "%s/DotNetLib.dll", root_path);
    char dotnet_type[]        = "DotNetLib.Lib, DotNetLib";
    char dotnet_type_method[] = "Main";

    // <SnippetLoadAndGet>
    // Function pointer to managed delegate
    component_entry_point_fn csharp_main = nullptr;
    int rc = load_assembly_and_get_function_pointer(
        dotnetlib_path,
        dotnet_type,
        dotnet_type_method,
        nullptr /*delegate_type_name*/,
        nullptr,
        (void**)&csharp_main);
    // </SnippetLoadAndGet>

    assert(rc == 0 && csharp_main != nullptr && \
        "Failure: load_assembly_and_get_function_pointer()");

    //
    // STEP 4: Run managed code
    //
    struct lib_args
    {
        int number1;
        int number2;
        int Result;
    };

    for (i = 0; i < 3; ++i)
    {
        // <SnippetCallManaged>
        struct lib_args args =  { .number1 = i, .number2 = i };
        csharp_main(&args, sizeof(args));
        printf("Sum from C#: %d\n",args.Result);
        // </SnippetCallManaged>
    }

//// DOTNET-HOST-SAMPLE STUFF ///////////////////////////////////////


    }
    PG_CATCH();
    {
        // Exception handling 
        PG_RE_THROW();
    }
    PG_END_TRY();

    if (SPI_finish() != SPI_OK_FINISH)
        elog(ERROR, "[pldotnet]: could not disconnect from SPI manager");
    PG_RETURN_VOID();
}
#endif






