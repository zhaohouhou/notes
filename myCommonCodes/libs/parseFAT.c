//simple parser for Mach-O FAT executive
//See low-level/Mach-O_Executive.md for more details.
//edited by zhaohouhou
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mach/machine.h>  //cpu_type_t, cpu_subtype_t

///////////////////////////////
//Definitions in <mach-o/fat.h>.
#define FAT_MAGIC   0xcafebabe
#define FAT_CIGAM   0xbebafeca  /* NXSwapLong(FAT_MAGIC) */

struct fat_header {
    uint32_t magic; /* FAT_MAGIC */
    uint32_t nfat_arch; /* number of structs that follow */
};

struct fat_arch {
    cpu_type_t cputype; /* cpu specifier (int) */
    cpu_subtype_t cpusubtype; /* machine specifier (int) */
    uint32_t offset;    /* file offset to this object file */
    uint32_t size;      /* size of this object file */
    uint32_t align;     /* alignment as a power of 2 */
};

///////////////////////////////
//Definitions in <mach-o/loader.h>.
struct mach_header {
    uint32_t magic;       /* mach magic number identifier */
    cpu_type_t cputype;   /* cpu specifier */
    cpu_subtype_t cpusubtype;   /* machine specifier */
    uint32_t filetype;   /* type of file */
    uint32_t ncmds;       /* number of load commands */
    uint32_t sizeofcmds;   /* the size of all the load commands */
    uint32_t flags;       /* flags */
};
#define MH_MAGIC    0xfeedface    /* the mach magic number */
#define MH_CIGAM    0xcefaedfe    /* NXSwapInt(MH_MAGIC) */

struct mach_header_64 {
    uint32_t magic;       /* mach magic number identifier */
    cpu_type_t cputype;   /* cpu specifier */
    cpu_subtype_t cpusubtype;   /* machine specifier */
    uint32_t filetype;   /* type of file */
    uint32_t ncmds;       /* number of load commands */
    uint32_t sizeofcmds;   /* the size of all the load commands */
    uint32_t flags;       /* flags */
    uint32_t reserved;   /* reserved */
};
#define MH_MAGIC_64 0xfeedfacf /* the 64-bit mach magic number */
#define MH_CIGAM_64 0xcffaedfe /* NXSwapInt(MH_MAGIC_64) */

struct load_command {
    uint32_t cmd;    /* type of load command */
    uint32_t cmdsize; /* total size of command in bytes */
};

typedef int vm_prot_t;
struct segment_command { /* for 32-bit architectures */
    uint32_t cmd;       /* LC_SEGMENT */
    uint32_t cmdsize;   /* includes sizeof section structs */
    char segname[16];       /* segment name */
    uint32_t vmaddr;       /* memory address of this segment */
    uint32_t vmsize;       /* memory size of this segment */
    uint32_t fileoff;   /* file offset of this segment */
    uint32_t filesize;   /* amount to map from the file */
    vm_prot_t maxprot;   /* maximum VM protection */
    vm_prot_t initprot;   /* initial VM protection */
    uint32_t nsects;       /* number of sections in segment */
    uint32_t flags;       /* flags */
};

struct segment_command_64 { /* for 64-bit architectures */
    uint32_t cmd;       /* LC_SEGMENT_64 */
    uint32_t cmdsize;   /* includes sizeof section_64 structs */
    char segname[16];       /* segment name */
    uint64_t vmaddr;       /* memory address of this segment */
    uint64_t vmsize;       /* memory size of this segment */
    uint64_t fileoff;   /* file offset of this segment */
    uint64_t filesize;   /* amount to map from the file */
    vm_prot_t maxprot;   /* maximum VM protection */
    vm_prot_t initprot;   /* initial VM protection */
    uint32_t nsects;       /* number of sections in segment */
    uint32_t flags;       /* flags */
};

struct section { /* for 32-bit architectures */
    char sectname[16];       /* name of this section */
    char segname[16];       /* segment this section goes in */
    uint32_t addr;       /* memory address of this section */
    uint32_t size;       /* size in bytes of this section */
    uint32_t offset;       /* file offset of this section */
    uint32_t align;       /* section alignment (power of 2) */
    uint32_t reloff;       /* file offset of relocation entries */
    uint32_t nreloc;       /* number of relocation entries */
    uint32_t flags;       /* flags (section type and attributes)*/
    uint32_t reserved1;   /* reserved (for offset or index) */
    uint32_t reserved2;   /* reserved (for count or sizeof) */
};

struct section_64 { /* for 64-bit architectures */
    char sectname[16];       /* name of this section */
    char segname[16];       /* segment this section goes in */
    uint64_t addr;       /* memory address of this section */
    uint64_t size;       /* size in bytes of this section */
    uint32_t offset;       /* file offset of this section */
    uint32_t align;       /* section alignment (power of 2) */
    uint32_t reloff;       /* file offset of relocation entries */
    uint32_t nreloc;       /* number of relocation entries */
    uint32_t flags;       /* flags (section type and attributes)*/
    uint32_t reserved1;   /* reserved (for offset or index) */
    uint32_t reserved2;   /* reserved (for count or sizeof) */
    uint32_t reserved3;   /* reserved */
};

/////////////////////////////
//Global data structures
int NEED_SWAP = 0;
uint32_t nfat_arch;
struct fat_arch* fat_arch_list;

/////////////////////////////
//Print Utilities
#define PRINT_PADDING_LENGTH 36

void format_print_uint32(const char* item_name, uint32_t value) {
    int colum_len = PRINT_PADDING_LENGTH - 8;
    printf("%s", item_name);
    colum_len -= strlen(item_name);
    if (colum_len < 0) {
        printf("\t%08x\n", value);
        return;
    }
    while(colum_len >0) {
        colum_len -= 1;
        printf(" ");
    }
    printf("%08x\n", value);
}

void format_print_uint64(const char* item_name, uint64_t value) {
    int colum_len = PRINT_PADDING_LENGTH - 16;
    printf("%s", item_name);
    colum_len -= strlen(item_name);
    if (colum_len < 0) {
        printf("\t%016llx\n", value);
        return;
    }
    while(colum_len >0) {
        colum_len -= 1;
        printf(" ");
    }
    printf("%016llx\n", value);
}

void format_print_str(const char* item_name, const char* value) {
    int colum_len = PRINT_PADDING_LENGTH;
    printf("%s", item_name);
    colum_len -= strlen(item_name);
    colum_len -= strlen(value);
    if (colum_len < 0) {
        printf("\t%s\n", value);
        return;
    }
    while(colum_len >0) {
        colum_len -= 1;
        printf(" ");
    }
    printf("%s\n", value);
}

void print_separator() {
    printf("===================================\n");
}

/////////////////////////////
//Handle Layout Utilities
const char *get_machine(uint32_t filetype) {
    switch (filetype) {
    case CPU_TYPE_I386:
        return "X86";
    case CPU_TYPE_X86_64:
        return "X86_64";
    case CPU_TYPE_ARM:
        return "ARM";
    case CPU_TYPE_ARM64:
        return "ARM64";
    case CPU_TYPE_POWERPC:
        return "PPC";
    case CPU_TYPE_POWERPC64:
        return "PPC64";
    default:
        return "?";
    }
}

#define    MH_OBJECT    0x1        /* relocatable object file */
#define    MH_EXECUTE    0x2        /* demand paged executable file */
#define    MH_FVMLIB    0x3        /* fixed VM shared library file */
#define    MH_CORE        0x4        /* core file */
#define    MH_PRELOAD    0x5        /* preloaded executable file */
#define    MH_DYLIB    0x6        /* dynamically bound shared library */
#define    MH_DYLINKER    0x7        /* dynamic link editor */
#define    MH_BUNDLE    0x8        /* dynamically bound bundle file */
const char *get_filetype(uint32_t filetype) {
    switch (filetype) {
    case MH_OBJECT:
        return "MH_OBJECT";
    case MH_EXECUTE:
        return "MH_EXECUTE";
    case MH_FVMLIB:
        return "MH_FVMLIB";
    case MH_DYLIB:
        return "MH_DYLIB";
    case MH_DYLINKER:
        return "MH_DYLINKER";
    case MH_BUNDLE:
        return "MH_BUNDLE";
    default:
        return "other(cannot handle)";
    }
}

#define    LC_SEGMENT    0x1    /* segment of this file to be mapped */
#define    LC_SEGMENT_64    0x19    /* segment of this file to be mapped */
const char *get_cmdtype(uint32_t cmd) {
    switch (cmd) {
    case LC_SEGMENT:
        return "LC_SEGMENT";
    case LC_SEGMENT_64:
        return "LC_SEGMENT_64";
    default:
        return "other(not handled yet)";
    }
}

void swap32(uint32_t *data) {
    //swap data if needed
    if (!NEED_SWAP)
        return;
    uint32_t buffer = *data;
    unsigned char* ptr = (unsigned char*)data;
    unsigned char* src = (unsigned char*)&buffer;
    ptr[0] = src[3]; ptr[1] = src[2]; ptr[2] = src[1]; ptr[3] = src[0];
}

void swap(uint32_t *data, unsigned int size) {
    //size: in bytes, must be multiple of 4.
    if (!NEED_SWAP)
        return;
    for(unsigned int i = 0; i*4 < size; i++)
        swap32(data+i);
}

/////////////////////////
// Parsing
void dump_mach_64(uint32_t mach_index, unsigned char* data) {
    //1) Parse mach_header (magic is already dumped)
    struct mach_header_64* header = (struct mach_header_64*)data;
    if(header->magic == MH_CIGAM_64)
        NEED_SWAP = 1; //endian is reversed
    else
        NEED_SWAP = 0; //might be different among obj files
    //Swap this file once and for all
    swap((uint32_t *)data,
      sizeof(unsigned char)*fat_arch_list[mach_index].size);
    format_print_str("filetype", get_filetype(header->filetype));
    uint32_t ncmds = header->ncmds;
    format_print_uint32("number of commands", ncmds);

    //2) Parse load commands (searching for the __TEXT segment)
    unsigned char* cmd_ptr = data + sizeof(struct mach_header_64);
    for (int i = 0; i < ncmds; i++) {
        printf("\nsegment %d:\n", i);
        struct load_command *cmd_head = (struct load_command *)cmd_ptr;
        format_print_str("cmdtype", get_cmdtype(cmd_head->cmd));
        format_print_uint32("cmdsize", cmd_head->cmdsize);
        if(cmd_head->cmd == LC_SEGMENT_64) {//only handle 'segment' for now
            struct segment_command_64 *segment = (struct segment_command_64*) cmd_head;
            format_print_str("segname", segment->segname);
            format_print_uint64("vmaddr", segment->vmaddr);
            format_print_uint64("vmsize", segment->vmsize);
            format_print_uint64("fileoff", segment->fileoff);
            format_print_uint64("filesize", segment->filesize);
            uint32_t nsects = segment->nsects;
            format_print_uint32("nsects", nsects);
            if(0 == strcmp(segment->segname, "__TEXT")) {
                //3) Parse sections
                unsigned char* sect_ptr =
                    (unsigned char*)segment + sizeof(struct segment_command_64);
                for (int j = 0; j < nsects; j++) {
                    struct section_64 *sect_head = (struct section_64*) sect_ptr;
                    format_print_str("  sectname", sect_head->sectname);
                    format_print_str("  segname", sect_head->segname);
                    format_print_uint64("  addr", sect_head->addr);
                    format_print_uint64("  size", sect_head->size);
                    format_print_uint32("  file_offset", sect_head->offset);
                    sect_ptr += sizeof(struct section_64);//sect_head->size;
                    printf("\n");
                }
            }
        }
        cmd_ptr += cmd_head->cmdsize;
    }
}

void dump_mach(uint32_t mach_index, unsigned char* data) {
    //1) Parse mach_header
    struct mach_header* header = (struct mach_header*)data;
    format_print_uint32("magic", header->magic);
    if(header->magic == MH_MAGIC_64 || header->magic == MH_CIGAM_64) { //64-bit object file
        dump_mach_64(mach_index, data);
        return;
    }
    if(header->magic == MH_CIGAM)
        NEED_SWAP = 1; //endian is reversed
    else
        NEED_SWAP = 0; //might be different among obj files!!
    //Swap this file once and for all
    swap((uint32_t *)data,
        sizeof(unsigned char)*fat_arch_list[mach_index].size);
    format_print_str("filetype", get_filetype(header->filetype));
    uint32_t ncmds = header->ncmds;
    format_print_uint32("number of commands", ncmds);

    //2) Parse load commands (searching for the __TEXT segment)
    unsigned char* cmd_ptr = data + sizeof(struct mach_header);
    for (int i = 0; i < ncmds; i++) {
        printf("\nsegment %d:\n", i);
        struct load_command *cmd_head = (struct load_command *)cmd_ptr;
        format_print_str("cmdtype", get_cmdtype(cmd_head->cmd));
        format_print_uint32("cmdsize", cmd_head->cmdsize);
        if(cmd_head->cmd == LC_SEGMENT) {//only handle 'segment' for now
            struct segment_command *segment = (struct segment_command*) cmd_head;
            format_print_str("segname", segment->segname);
            format_print_uint32("vmaddr", segment->vmaddr);
            format_print_uint32("vmsize", segment->vmsize);
            format_print_uint32("fileoff", segment->fileoff);
            format_print_uint32("filesize", segment->filesize);
            uint32_t nsects = segment->nsects;
            format_print_uint32("nsects", nsects);
            if(0 == strcmp(segment->segname, "__TEXT")) {
                //3) Parsing sections
                unsigned char* sect_ptr =
                    (unsigned char*)segment + sizeof(struct segment_command);
                for (int j = 0; j < nsects; j++) {
                    struct section *sect_head = (struct section*) sect_ptr;
                    format_print_str("  sectname", sect_head->sectname);
                    format_print_str("  segname", sect_head->segname);
                    format_print_uint32("  addr", sect_head->addr);
                    format_print_uint32("  size", sect_head->size);
                    format_print_uint32("  file_offset", sect_head->offset);
                    sect_ptr += sizeof(struct section);
                    printf("\n");
                }
            }
        }
        cmd_ptr += cmd_head->cmdsize;
    }
}

void dump_fat(FILE *file) {
    //1) parse fat_header
    uint32_t magic;
    fseek(file, 0, SEEK_SET); //SEEK_SET: seek from file head.
    fread(&magic, sizeof(uint32_t), 1, file);
    format_print_uint32("fat_magic", magic);
    if(magic == FAT_CIGAM)
        NEED_SWAP = 1; //endian is reversed
    else if(magic != FAT_MAGIC) { // not a FAT file
        printf("Not a FAT file!\n");
        exit(0);
    }
    fread(&nfat_arch, sizeof(uint32_t), 1, file);
    swap32(&nfat_arch);
    format_print_uint32("nfat_arch", nfat_arch);
    //parse fat_arch structs
    fat_arch_list =
        (struct fat_arch *)malloc(sizeof(struct fat_arch)* nfat_arch);
    for (int i = 0; i < nfat_arch; i++) {
        struct fat_arch *new_arch = &fat_arch_list[i];
        fread(new_arch, sizeof(struct fat_arch), 1, file);
        swap((uint32_t *)new_arch, sizeof(struct fat_arch));
    }

    //2) parse Mach-O files
    for (int i = 0; i < nfat_arch; i++) {
        //copy file data to memory
        unsigned char* buffer = (unsigned char*)malloc(
            sizeof(unsigned char)*fat_arch_list[i].size);
        fseek(file, fat_arch_list[i].offset, SEEK_SET);
        fread(buffer,
            sizeof(unsigned char)*fat_arch_list[i].size, 1, file);
        print_separator();
        printf("Object %d:\n", i);
        format_print_str("cputype",
            get_machine(fat_arch_list[i].cputype));
        format_print_uint32("offset", fat_arch_list[i].offset);
        format_print_uint32("size", fat_arch_list[i].size);
        dump_mach(i, buffer);
        print_separator();
        free(buffer);
    }
    printf("\n");
}

/**
   argv[1]: input FAT file name
 */
int main(int argc, char* argv[]) {
    const char* file_name = argv[1];
    FILE *file = fopen(file_name, "rb");
    dump_fat(file);
    fclose(file);
}
