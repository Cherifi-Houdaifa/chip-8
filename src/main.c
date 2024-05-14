#include "hederz.h"


struct chip8* cpu;
struct ssdl sdl;


int main(int argc, char* argv[]) {
	initchip8(&cpu, argv[1]);
	initsdl(&sdl);
	while (1) {
		handleeventssdl(&sdl, cpu->keyboad);

		timerz(cpu);
		execute(cpu);

		updatesdl(&sdl, cpu->display);
	}

	return 0;
}

void initchip8 (struct chip8** pcpu, char* file) {
	*pcpu = calloc(1, sizeof(struct chip8));
	struct chip8* cpu = *pcpu;
	cpu->pc = 0x200;

	// set the fonts in memory
	#include "fonts" // the c preprocessor is the best

	FILE* fd = fopen(file, "r");
	fseek(fd, 0L, SEEK_END);
	int size = ftell(fd);
	fseek(fd, 0L, SEEK_SET);
	fread(cpu->mem + 0x200, 0x1, size, fd);
}

void timerz(struct chip8* cpu) {
	if (cpu->dt > 0) {
		cpu->dt = cpu->dt - 1;
	}

	if (cpu->st > 0) {
		// do beep
		puts("beep");
		cpu->st = cpu->st - 1;
	}
}


// returns instruction at pc
uint16_t fetch(struct chip8* cpu) {
	uint16_t instruction;
	instruction = (cpu->mem[cpu->pc] << 8) + cpu->mem[cpu->pc+1];
	cpu->pc = cpu->pc + 2;
	return instruction;
}

// used for reversing the bits of a byte
const uint8_t revtable[] = {
        0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
        0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
        0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
        0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
        0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
        0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
        0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
        0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
        0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
        0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
        0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
        0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
        0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
        0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
        0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
        0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
        0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
        0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
        0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
        0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
        0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
        0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
        0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
        0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
        0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
        0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
        0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
        0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
        0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
        0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
        0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
        0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff,
    };

void execute(struct chip8* cpu) {
	uint16_t instruction = fetch(cpu);
	
	uint8_t x, y, kk, n;
	uint8_t tx, ty; // tmp vars to store vx and vy
	uint16_t nnn;

	x = (instruction & 0xf00) >> 8;
	y = (instruction & 0xf0) >> 4;
	kk = (instruction & 0xff);
	n = (instruction & 0xf);
	nnn = (instruction & 0xfff);


	// parse and execute instructions here (probably should seperate them)
	if (instruction == 0xe0) {
		// CLS
		// puts("cls");
		for (int j = 0; j < 0x20; j++) 
			cpu->display[j] = 0;
	} else if (instruction == 0xee) {
		// RET
		// puts("ret");
		cpu->pc = cpu->stack[cpu->sp];
		cpu->sp = cpu->sp - 1;
	} else if ((instruction & 0xf000) == 0x1000) {
		// JP
		// puts("jp nnn");
		cpu->pc = nnn;
	} else if ((instruction & 0xf000) == 0x2000) {
		// CALL
		// puts("call nnn");
		cpu->sp = cpu->sp + 1;
		cpu->stack[cpu->sp] = cpu->pc;
		cpu->pc = nnn;
	} else if ((instruction & 0xf000) == 0x3000) {
		// SE
		// puts("se vx, kk");
		if (cpu->vn[x] == kk) {
			cpu->pc = cpu->pc + 2;
		}
	} else if ((instruction & 0xf000) == 0x4000) {
		// SNE
		// puts("sne vx, kk");
		if (cpu->vn[x] != kk) {
			cpu->pc = cpu->pc + 2;
		}
	} else if ((instruction & 0xf00f) == 0x5000) {
		// SE (with registers)
		// puts("se vx, vy");
		if (cpu->vn[x] == cpu->vn[y]) {
			cpu->pc = cpu->pc + 2;
		}
	} else if ((instruction & 0xf000) == 0x6000) {
		// LD
		// puts("ld vx, kk");
		cpu->vn[x] = kk;

	} else if ((instruction & 0xf000) == 0x7000) {
		// ADD
		// puts("add vx, kk");
		cpu->vn[x] = cpu->vn[x] + kk;
	} else if ((instruction & 0xf00f) == 0x8000) {
		// LD (with registers)
		// puts("ld vx, vy");
		cpu->vn[x] = cpu->vn[y];
	} else if ((instruction & 0xf00f) == 0x8001) {
		// OR
		// puts("or vx, vy");
		cpu->vn[x] = cpu->vn[x] | cpu->vn[y];
	} else if ((instruction & 0xf00f) == 0x8002) {
		// AND
		// puts("and vx, vy");
		cpu->vn[x] = cpu->vn[x] & cpu->vn[y];
	} else if ((instruction & 0xf00f) == 0x8003) {
		// XOR
		// puts("xor vx, vy");
		cpu->vn[x] = cpu->vn[x] ^ cpu->vn[y];
	} else if ((instruction & 0xf00f) == 0x8004) {
		// ADD (with registers)
		// puts("add vx, vy");
		tx = cpu->vn[x];
		ty = cpu->vn[y];

		cpu->vn[x] = cpu->vn[x] + cpu->vn[y];

		if (tx + ty > 255)
			cpu->vn[0xf] = 1;
		else 
			cpu->vn[0xf] = 0;
	} else if ((instruction & 0xf00f) == 0x8005) {
		// SUB
		// puts("sub vx, vy");
		
		tx = cpu->vn[x];
		ty = cpu->vn[y];

		cpu->vn[x] = cpu->vn[x] - cpu->vn[y];

		if (tx >= ty)
			 cpu->vn[0xf] = 1;
		else 
			cpu->vn[0xf] = 0;
	} else if ((instruction & 0xf00f) == 0x8006) {
		// SHR
		// puts("shr vx");
		tx = cpu->vn[x];
		cpu->vn[x] = cpu->vn[x] >> 1;

		cpu->vn[0xf] = tx & 0x1;
	} else if ((instruction & 0xf00f) == 0x8007) {
		// SUBN
		// puts("vx = vy - vx");
		tx = cpu->vn[x];
		ty = cpu->vn[y];

		cpu->vn[x] = cpu->vn[y] - cpu->vn[x];

		if (ty >= tx)
			 cpu->vn[0xf] = 1;
		else 
			cpu->vn[0xf] = 0;
	} else if ((instruction & 0xf00f) == 0x800e) {
		// SHL
		// puts("shl vx");
		tx = cpu->vn[x];

		cpu->vn[x] = cpu->vn[x] << 1;

		cpu->vn[0xf] = (tx >> 7);
	} else if ((instruction & 0xf00f) == 0x9000) {
		// SNE (with registers)
		// puts("sne vx, vy");
		if (cpu->vn[x] != cpu->vn[y]) {
			cpu->pc = cpu->pc + 2;
		}
	} else if ((instruction & 0xf000) == 0xa000) {
		// LD I
		// puts("ld i, nnn");
		cpu->i = nnn;
	} else if ((instruction & 0xf000) == 0xb000) {
		// JP v0, nnn
		// puts("jp v0, nnn (pc = v0 + nnn)");
		cpu->pc = cpu->vn[0x0] + nnn;
	} else if ((instruction & 0xf000) == 0xc000) {
		// RND vx, byte
		// puts("rnd vx, mask");
		cpu->vn[x] = (rand() % 256) & kk;
	} else if ((instruction & 0xf000) == 0xd000) {
		// DRW vx, vy, n
		// puts("DRW vx, vy, n");
		uint8_t* sprite = malloc(n * sizeof(uint8_t));
		uint8_t flag = 0;
		// read sprite
		for (int j = 0; j < n; j++) {
			sprite[j] = cpu->mem[cpu->i + j];
		}
		for (int j = 0; j < n; j++) {
			uint64_t mask = (uint64_t)revtable[sprite[j]];
			mask = (mask << (cpu->vn[x] % 64)) | (mask >> (64 - (cpu->vn[x] % 64)));
			if ((mask & (cpu->display[(cpu->vn[y] + j) % 0x20])) != 0)
				flag = 1;
			cpu->display[(cpu->vn[y] + j) % 0x20] ^= mask;
		}
		cpu->vn[0xf] = flag;
		free(sprite);
	} else if ((instruction & 0xf0ff) == 0xe09e) {
		// SKP
		// puts("SKP vx, k");
		if (cpu->keyboad[cpu->vn[x]])
			cpu->pc = cpu->pc + 2;
	} else if ((instruction & 0xf0ff) == 0xe0a1) {
		// SKNP
		// puts("SKNP vx, k");
		if (!cpu->keyboad[cpu->vn[x]])
			cpu->pc = cpu->pc + 2;
	} else if ((instruction & 0xf0ff) == 0xf007) {
		// LD vx, dt
		// puts("LD vx, dt");
		cpu->vn[x] = cpu->dt;
	} else if ((instruction & 0xf0ff) == 0xf00a) {
		// LD vx, K
		// puts("LD vx, K");
		cpu->vn[x] = waitforkey(&sdl);
		printf("key: %x\n", cpu->vn[x]);
	} else if ((instruction & 0xf0ff) == 0xf015) {
		// LD dt, vx
		// puts("LD dt, vx");
		cpu->dt = cpu->vn[x];
	} else if ((instruction & 0xf0ff) == 0xf018) {
		// LD st, vx
		// puts("LD st, vx");
		cpu->st = cpu->vn[x];
	} else if ((instruction & 0xf0ff) == 0xf01e) {
		// ADD i, vx
		// puts("ADD i, vx");
		cpu->i = cpu->i + cpu->vn[x];
	} else if ((instruction & 0xf0ff) == 0xf029) {
		// LD f, vx
		// puts("LD f, vx");
		cpu->i = cpu->vn[x] * 0x5;
	} else if ((instruction & 0xf0ff) == 0xf033) {
		// LD B, vx
		// puts("bcd");
		uint8_t h, t, o; // hundreds, tens and ones
		h = cpu->vn[x] / 100;
		t = cpu->vn[x] / 10 - (h * 10);
		o = cpu->vn[x] - (t * 10) - (h * 100);
		cpu->mem[cpu->i] = h;
		cpu->mem[cpu->i + 1] = t;
		cpu->mem[cpu->i + 2] = o;
	} else if ((instruction & 0xf0ff) == 0xf055) {
		// LD [i], vx
		// puts("regdump");
		for (int j = 0; j <= x; j++) {
			cpu->mem[cpu->i + j] = cpu->vn[j];
		}
	} else if ((instruction & 0xf0ff) == 0xf065) {
		// LD vx, [i]
		// puts("regload");
		for (int j = 0; j <= x; j++) {
			cpu->vn[j] = cpu->mem[cpu->i + j];
		}
	} else {
		puts("error happened");
		printf("pc: %x\n", cpu->pc);
		printf("instruction: %x\n", instruction);
		exit(1);
	}
}