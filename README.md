# CSC3050 Project3 5 Stage CPU Simulator

This is a forked version of Hao He's [RISC-V Simulator](https://github.com/hehao98/RISCV-Simulator). In this repository, using pages to manage memory is disabled.

## Summary
- [x] Part 1 correctness of supporting RISCV32I
- [x] Part 1 correctness of fused instructions with data forwarding
- [x] Part 1 correctness of all instructions without data forwarding
- [x] Part 2 correctness of part2 p2.s
- [x] Part 2 efficiency of part2 p2.s
- [x] A short report about anything you have learn in this project
- [x] Part 2 correctness of part2 p3.s
- [x] Part 2 efficiency of part2 p3.s
- [x] Improve the logic of Hao He’s simulator. (using Correlated Branch Prediction)

## Improve:using Correlated Branch Prediction

The idea is combine `PC` and a global branch history(GHB) to indexing BHT. (More Details in *DataHazards* slides)

### Implementation
the implementation is not difficult based on existing code. 
There are 3 tasks to do:
- Maintain a global branch history `ghb` in class `BranchPredictor`
- Update `ghb` everytime branch happens `this->ghb = (this->ghb << 1) + 1` or ` this->ghb = (this->ghb << 1)`
- when indexing do **xor** operation between `pc` and `ghb` (Gshare) `id = (pc ^ this->ghb) % PRED_BUF_SIZE`

### Results
I implement different length of `ghb` (32/16/8/4)

| Program | BPB | CBP32 | CBP16 | CBP8 | CBP4 |
| - | -| - | - | - | - |
| helloworld.riscv | 0.8615 | 0.8615 | 0.8615 | 0.8615 | 0.8615 |
| quicsort.riscv | 0.9589 | 0.9829 | 0.9829 | 0.9844 | 0.9763 |
| matrixmult.riscv | 0.6150 | 0.7894 | 0.7894 | 0.8450 | 0.8533 |
| ackermann.riscv | 0.9595 | 0.9809 | 0.9809 | 0.9814 | 0.9805 |

The results show that correlated branch prediction can further improve performance based on the 2-bit branch prediction buffer, especially in *matrixmult.riscv*. However, there is no performance improvement in *helloworld.riscv*, likely because the branch behavior is relatively simple.

From the results of *ackermann.riscv* and *quicksort.riscv*, we observe that as the length of the global branch history register (GHR) decreases, the improvement **first increases and then decreases**. The reason could be that if the GHR is too short, it fails to capture sufficient historical context, making it unable to reflect correlations between distant branches. Conversely, if the GHR is too long, it may capture irrelevant distant branch behaviors, leading to history pollution.

> Notice that the branch prediction results differ from the results shown in **He Hao's blog**. I checked the initial execution results of the simulator and found that they were already different from the beginning. The reason could be that our project compiles C code using **RISC-V32** instead of **RISC-V64**.

## Challenges faced during implementation

In whole project, switch RISCV64 to RISCV32 is relative easy based on He Hao's code, but when implement pipline without dataforwarding, i met some tricky bugs:
1. **dReg stall**: when dReg is stalled, the data got from register file is also freezed, we need to update `dReg` at correct timing to ensure correctness.
2. **zero register**: if the current instruction has x0 as destReg and next instruction also used x0 as rs, it is easy to be detected as data hazard(but actually not) 
3. **jump eliminated dataHazard**: if jump happens, pipline is drained, therefore eliminate some dataHazard.

## What I learn

1. **Project 3** greatly enhanced my programming and debugging skills. Although I encountered some challenging bugs during development, the process of resolving them provided me with valuable debugging experience and reinforced the importance of writing robust code with comprehensive error handling.  

2. While understanding **He Hao's simulator implementation**, I further solidified my knowledge of the **5-stage pipeline** and how data is transferred between different stages through **pipeline registers**.

## Part 2 Results

### CPU STATE (Correctness)

```bash
# For part2 and part2_p2
------------ CPU STATE ------------
PC: 0x10118
zero: 0x00000000(0) ra: 0x00000001(1) sp: 0x80000000(-2147483648) gp: 0x00000000(0) 
tp: 0x00000000(0) t0: 0x00000004(4) t1: 0x00001000(4096) t2: 0x00001000(4096) 
s0: 0x00000000(0) s1: 0x00000000(0) a0: 0x00000000(0) a1: 0x0000000d(13) 
a2: 0x0000000e(14) a3: 0x000002d8(728) a4: 0x0000009c(156) a5: 0x0000000e(14) 
a6: 0x0016c2d8(1491672) a7: 0x0000005d(93) s2: 0x00000000(0) s3: 0x00000000(0) 
s4: 0x00000000(0) s5: 0x00000000(0) s6: 0x00000000(0) s7: 0x00000000(0) 
s8: 0x00000000(0) s9: 0x00000000(0) s10: 0x00000000(0) s11: 0x00000000(0) 
t3: 0x00000000(0) t4: 0x00000000(0) t5: 0x00000000(0) t6: 0x00000000(0) 
-----------------------------------

# For part2_p3
------------ CPU STATE ------------
PC: 0x10110
zero: 0x00000000(0) ra: 0x00000001(1) sp: 0x80000000(-2147483648) gp: 0x00000000(0) 
tp: 0x00000000(0) t0: 0x00000004(4) t1: 0x00001000(4096) t2: 0x00001000(4096) 
s0: 0x00000000(0) s1: 0x00000000(0) a0: 0x00000000(0) a1: 0x0000000d(13) 
a2: 0x0000000e(14) a3: 0x000002d8(728) a4: 0x00000010(16) a5: 0x0000000e(14) 
a6: 0x0016c2d8(1491672) a7: 0x0000005d(93) s2: 0x00000000(0) s3: 0x00000000(0) 
s4: 0x00000000(0) s5: 0x00000000(0) s6: 0x00000000(0) s7: 0x00000000(0) 
s8: 0x00000000(0) s9: 0x00000000(0) s10: 0x00000000(0) s11: 0x00000000(0) 
t3: 0x00000000(0) t4: 0x00000000(0) t5: 0x00000000(0) t6: 0x00000000(0) 
-----------------------------------
```

Notice here `a4` register is different in part2/par2_p2 and part2_p3, the reason is because `fmadd.i a3, a1, a2, a3` is used to switch `mul a4, a1, a2`  and `add a3, a3, a4`, so `a4` is eliminated in loop, so it no longer update after `li a4, 16`.

### STATISTICS(Efficiency)

```bash
student@8e33fa974021:~/CSC3050-2025-Spring-Project-3/build$ ./Simulator ../riscv-elf/part2.riscv -x 
Program exit from an exit() system call
------------ STATISTICS -----------
Number of Instructions: 270
Number of Cycles: 600
Avg Cycles per Instrcution: 2.2222
Branch Perdiction Accuacy: 0.0833 (Strategy: Always Not Taken)
Number of Control Hazards: 23
Number of Data Hazards: 101
Number of Memory Hazards: 0
-----------------------------------
student@8e33fa974021:~/CSC3050-2025-Spring-Project-3/build$ ./Simulator ../riscv-elf/part2_p2.riscv -x 
Program exit from an exit() system call
------------ STATISTICS -----------
Number of Instructions: 270
Number of Cycles: 514
Avg Cycles per Instrcution: 1.9037
Branch Perdiction Accuacy: 0.0833 (Strategy: Always Not Taken)
Number of Control Hazards: 23
Number of Data Hazards: 64
Number of Memory Hazards: 0
-----------------------------------
student@8e33fa974021:~/CSC3050-2025-Spring-Project-3/build$ ./Simulator ../riscv-elf/part2_p3.riscv -x 
Program exit from an exit() system call
------------ STATISTICS -----------
Number of Instructions: 246
Number of Cycles: 502
Avg Cycles per Instrcution: 2.0407
Branch Perdiction Accuacy: 0.0833 (Strategy: Always Not Taken)
Number of Control Hazards: 23
Number of Data Hazards: 64
Number of Memory Hazards: 0
-----------------------------------
```

Notice Cycles is reduced after code scheduling and further deduced after using `fmadd.i` to reduce instruction numbers.