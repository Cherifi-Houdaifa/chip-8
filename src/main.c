#include "hederz.h"


struct chip8* cpu;

int main(int argc, char* args[]) {
	cpu = malloc(sizeof(struct chip8));
	
  	return 0;
}


// returns instruction at pc
uint16_t fetch(struct chip8* cpu) {
	uint16_t instruction;
	instruction = (cpu->mem[cpu->pc] << 8) + cpu->mem[cpu->pc+1];
	return instruction;
}

void execute(struct chip8* cpu) {
	uint16_t instruction = fetch(cpu);
	
	uint8_t x, y, kk, n;
	uint16_t nnn;

	x = (instruction & 0xf00) >> 8;
	y = (instruction & 0xf0) >> 4;
	kk = (instruction & 0xff);
	n = (instruction & 0xf);
	nnn = (instruction & 0xfff);

	// parse and execute instructions here (probably should seperate them)
	if ((instruction & 0xf000) == 0x0000) {
		// ignored
	} else if (instruction == 0xe0) {
		// CLS
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 0x20; j++) {
				cpu->display[i][j] = 0;
			}
		}
	} else if (instruction == 0xee) {
		// RET
		cpu->pc = cpu->stack[cpu->sp];
		cpu->sp = cpu->sp - 1;
	} else if ((instruction & 0xf000) == 0x1000) {
		// JP
		cpu->pc = nnn;
	} else if ((instruction & 0xf000) == 0x2000) {
		// CALL
		cpu->sp = cpu->sp + 1;
		cpu->stack[cpu->sp] = cpu->pc;
		cpu->pc = nnn;
	} else if ((instruction & 0xf000) == 0x3000) {
		// SE
		if (cpu->vn[x] == kk) {
			cpu->pc = cpu->pc + 2;
		}
	} else if ((instruction & 0xf000) == 0x4000) {
		// SNE
		if (cpu->vn[x] != kk) {
			cpu->pc = cpu->pc + 2;
		}
	} else if ((instruction & 0xf00f) == 0x5000) {
		// SE (with registers)
		if (cpu->vn[x] == cpu->vn[y]) {
			cpu->pc = cpu->pc + 2;
		}
	} else if ((instruction & 0xf000) == 0x6000) {
		// LD
		cpu->vn[x] = kk;
	} else if ((instruction & 0xf000) == 0x7000) {
		// ADD
		cpu->vn[x] = cpu->vn[x] + kk;
	} else if ((instruction & 0xf00f) == 0x8000) {
		// LD (with registers)
		cpu->vn[x] = cpu->vn[y];
	} else if ((instruction & 0xf00f) == 0x8001) {
		// OR
		cpu->vn[x] = cpu->vn[x] | cpu->vn[y];
	} else if ((instruction & 0xf00f) == 0x8002) {
		// AND
		cpu->vn[x] = cpu->vn[x] & cpu->vn[y];
	} else if ((instruction & 0xf00f) == 0x8003) {
		// XOR
		cpu->vn[x] = cpu->vn[x] ^ cpu->vn[y];
	} else if ((instruction & 0xf00f) == 0x8004) {
		// ADD (with registers)
		cpu->vn[x] = cpu->vn[x] + cpu->vn[y];
		// TODO set flags register
	} else if ((instruction & 0xf00f) == 0x8005) {
		// SUB
		cpu->vn[x] = cpu->vn[x] - cpu->vn[y];
		// TODO set flags register
	} else if ((instruction & 0xf00f) == 0x8006) {
		// SHR
		cpu->vn[x] = cpu->vn[x] >> 1;
		// TODO set flags register
	} else if ((instruction & 0xf00f) == 0x8007) {
		// SUBN
		cpu->vn[x] = cpu->vn[y] - cpu->vn[x];
		// TODO set flags register
	} else if ((instruction & 0xf00f) == 0x800e) {
		// SHL
		cpu->vn[x] = cpu->vn[x] << 1;
		// TODO set flags register
	} else if ((instruction & 0xf00f) == 0x9000) {
		// SNE (with registers)
		if (cpu->vn[x] != cpu->vn[y]) {
			cpu->pc = cpu->pc + 2;
		}
	} else if ((instruction & 0xf000) == 0xa000) {
		// LD I
		cpu->i = nnn;
	} else if ((instruction & 0xf000) == 0xb000) {
		// JP v0, nnn
		cpu->pc = cpu->vn[0x0] + nnn;
	} else if ((instruction & 0xf000) == 0xc000) {
		// RND vx, byte
		cpu->vn[x] = (rand() % 256) & kk;
	}
}