# Cloud → local session handoff

Kept up to date by the cloud session after every committed file, so a local
session can resume from whatever was last pushed.

## Where things are

- Branch: `mainline-9uwr8v` on `origin` (github.com/nitrostemp/pokeheartgold-slop).
  Each file also has its own `decomp/<name>` branch. `mainline` itself has NOT
  been fast-forwarded to these commits yet — do that locally when you're ready:
  `git fetch origin && git checkout mainline && git merge --ff-only origin/mainline-9uwr8v`.
- Every pushed commit is compare-verified (HG + SS SHA1) and attested.
- Anything uncommitted in the cloud container is lost when the session ends;
  only what is pushed survives. In-progress work is noted below.

## Done in the cloud session (newest last)

| File | Result |
|---|---|
| overlay_41_02248400 | 45 C + 1 NONMATCHING (ov41_02248B48 regalloc) |
| unk_02026DE0 | 2/2 C |
| unk_0200FA24 | 32 C + 1 NONMATCHING (BeginNormalPaletteFade, duplicate pool word) |
| unk_0201010C | 127/127 C |
| render_window | 37 C + 3 NONMATCHING (sub_0200EA68, DrawPokemonPicFromSpecies/FromMon) |
| overlay_41_02248ED4 | 41/41 C |
| overlay_117 | 9/9 C |
| overlay_46 | 20/20 C |
| frontier/overlay_80_0222AEF8 | 54/54 C |
| unk_02096910 | 15/15 C |
| frontier/overlay_80_0222FD08 | 20 C + 1 NONMATCHING (ov80_022308C4 stack slot) |
| unk_02016EDC | 62/62 C (port of pokeplatinum pokemon_anim.c) |
| unk_020658D4 | 50/50 C (follow/effect-object movement; rodata static order fix) |
| overlay_01_022053EC | 40/40 C (follow-mon field helpers) |
| overlay_41_02247828 | 42/42 C (fashion case canvas + yes/no prompts) |
| unk_02032844 | 75/75 C (port of pokeplatinum wireless_manager.c) |
| unk_02034B0C | 56/56 C (port of pokeplatinum unk_02033200.c CommServerClient) |
| overlay_56 | 31/31 C (mail viewer, HG version of pokeplatinum mail_viewer.c) |
| overlay_41_02249A40 | 43/43 C (fashion case node list, bg scroll, button bar) |

## In progress / next

- Next target: whatever `tools/decomp_harness/next_target.sh --info` prints
  (the queue is rebuilt after every file).
- After that: `triage.py` queue (`next_target.sh --info`).

## Helpers in this directory

Run from the repo root.

- `show.py src/<tu>.c asm/<tu>.s <fn> [--asm|--c]` — print one function's asm and/or C.
- `sbs.py <tu-basename> <fn>` (basename may include a src subdir, e.g. `frontier/overlay_80_x`) — side-by-side asm vs compiled C after
  `compile_one.sh` (branch targets normalised, `!!` marks differences).
- `apply2.py src/<tu>.c patch.c` — replace function definitions with blocks from
  a patch file (`//@@ fn` headers); replaces a whole `#ifdef NONMATCHING` block
  when the function is inside one.
- `transcribe.py asm/<tu>.s <fn> label=replacement ...` — turn an asm function
  into MWCC inline asm with explicit `ldr rN, [pc, #off]` + `dcd` pool (use when
  the pool has duplicate words; MWCC dedups `ldr =X` literals).
- `variants.py src/<tu>.c <fn> < variants.json` — brute-force source variants.

## Lessons from this session (also in patterns.json)

- Disassembler labels inside .rodata/.data are often fields of ONE aggregate;
  model it as one static struct so MWCC keeps address order.
- `obj->f = Alloc(); p = obj->f;` (store then reload) is the common retail shape.
- MWCC inline asm: bare `[rN]` misassembles (use `[rN, #0]`); `.word` is
  rejected (use `dcd`).
- pret/pokeplatinum and pokediamond have matching C for many shared engine
  files; fetch via raw.githubusercontent.com.
- Toolchain downloads: github.com/.../raw is 403 behind the cloud proxy;
  raw.githubusercontent.com/pret/pokeheartgold/workflows/assets/ works.

After compile_one reports MATCH, also run `session_helpers/relocdiff.py <asm.o> <compile_one.o>`. objdiff masks relocation targets, so this is the only fast check for swapped identical statics.
