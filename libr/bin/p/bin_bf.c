/* radare - LGPL - Copyright 2013-2017 - pancake */

#include <r_types.h>
#include <r_util.h>
#include <r_lib.h>
#include <r_bin.h>

static void *load_bytes(RBinFile *arch, const ut8 *buf, ut64 sz, ut64 loadaddr, Sdb *sdb){
	return R_NOTNULL;
}

static bool load(RBinFile *arch) {
	return true;
}

static int destroy(RBinFile *arch) {
	return true;
}

static ut64 baddr(RBinFile *arch) {
	return 0;
}

static RList *strings(RBinFile *arch) {
	return NULL;
}

static RBinInfo *info(RBinFile *arch) {
	RBinInfo *ret = NULL;
	if (!(ret = R_NEW0 (RBinInfo))) {
		return NULL;
	}
	ret->lang = NULL;
	ret->file = arch->file? strdup (arch->file): NULL;
	ret->type = strdup ("brainfuck");
	ret->bclass = strdup ("1.0");
	ret->rclass = strdup ("program");
	ret->os = strdup ("any");
	ret->subsystem = strdup ("unknown");
	ret->machine = strdup ("brainfuck");
	ret->arch = strdup ("bf");
	ret->has_va = 1;
	ret->bits = 32; // 16?
	ret->big_endian = 0;
	ret->dbg_info = 0;
	/* TODO: move this somewhere else */
	eprintf ("f input 128 0x3000\n");
	eprintf ("o malloc://128 0x3000\n");
	eprintf ("f screen 80*25 0x4000\n");
	eprintf ("o malloc://80*25 0x4000\n");
	eprintf ("f stack 0x200 0x5000\n");
	eprintf ("o malloc://0x200 0x5000\n");
	eprintf ("f data 0x1000 0x6000\n");
	eprintf ("o malloc://0x1000 0x6000\n");
	eprintf ("ar\n"); // hack to init
	eprintf ("ar brk=stack\n");
	eprintf ("ar scr=screen\n");
	eprintf ("ar kbd=input\n");
	eprintf ("ar ptr=data\n");
	eprintf ("\"e cmd.vprompt=pxa 32@stack;pxa 32@screen;pxa 32@data\"\n");
	eprintf ("s 0\n");
	return ret;
}

static bool check_bytes(const ut8 *buf, ut64 length) {
	int i, is_bf = 0;
	if (buf && length > 0) {
		int max = R_MIN (16, length);
		const char *p = (const char *) buf;
		is_bf = 1;
		for (i = 0; i < max; i++) {
			switch (p[i]) {
			case '+':
			case '-':
			case '>':
			case '<':
			case '[':
			case ']':
			case ',':
			case '.':
			case ' ':
			case '\n':
			case '\r':
				break;
			default:
				is_bf = 0;
			}
		}
	}
	return is_bf;
}

static RList *entries(RBinFile *arch) {
	RList *ret;
	RBinAddr *ptr = NULL;

	if (!(ret = r_list_newf (free))) {
		return NULL;
	}
	if (!(ptr = R_NEW0 (RBinAddr))) {
		return ret;
	}
	ptr->paddr = ptr->vaddr = 0;
	r_list_append (ret, ptr);
	return ret;
}

struct r_bin_plugin_t r_bin_plugin_bf = {
	.name = "bf",
	.desc = "brainfuck",
	.license = "LGPL3",
	.load = &load,
	.load_bytes = &load_bytes,
	.destroy = &destroy,
	.check_bytes = &check_bytes,
	.baddr = &baddr,
	.entries = entries,
	.strings = &strings,
	.info = &info,
};

#ifndef CORELIB
struct r_lib_struct_t radare_plugin = {
	.type = R_LIB_TYPE_BIN,
	.data = &r_bin_plugin_bf,
	.version = R2_VERSION
};
#endif
