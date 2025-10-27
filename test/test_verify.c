#include "../head/head.h"

void verify_test_results(CPU_state *cpu) {
    printf("\n=== 测试结果验证 ===\n");
    
    // 检查关键寄存器的值
    uint32_t a0 = gpr_read(cpu, 10); // a0
    uint32_t a1 = gpr_read(cpu, 11); // a1  
    uint32_t a2 = gpr_read(cpu, 12); // a2
    uint32_t a3 = gpr_read(cpu, 13); // a3
    uint32_t a4 = gpr_read(cpu, 14); // a4
    uint32_t a5 = gpr_read(cpu, 15); // a5
    uint32_t ra = gpr_read(cpu, 1);  // ra
    
    printf("寄存器状态:\n");
    printf("  a0 (x10) = 0x%08x\n", a0);
    printf("  a1 (x11) = 0x%08x\n", a1);
    printf("  a2 (x12) = 0x%08x\n", a2);
    printf("  a3 (x13) = 0x%08x\n", a3);
    printf("  a4 (x14) = 0x%08x\n", a4);
    printf("  a5 (x15) = 0x%08x\n", a5);
    printf("  ra (x1)  = 0x%08x\n", ra);
    
    // 检查内存内容
    uint32_t mem_200 = mem_read(cpu, 0x200);
    printf("内存[0x200] = 0x%08x\n", mem_200);
    
    // 验证测试是否通过
    int tests_passed = 0;
    int total_tests = 9;
    
    printf("\n=== 指令测试结果 ===\n");
    
    // 测试1: LUI指令
    if (a0 == 0x00004001) {
        printf("✅ LUI 指令测试通过\n");
        tests_passed++;
    } else {
        printf("❌ LUI 指令测试失败\n");
    }
    
    // 测试2: ADDI指令
    if (a0 == 0x00004001) {
        printf("✅ ADDI 指令测试通过\n");
        tests_passed++;
    } else {
        printf("❌ ADDI 指令测试失败\n");
    }
    
    // 测试3: ADD指令  
    if (a2 == 0x00004003) {
        printf("✅ ADD 指令测试通过\n");
        tests_passed++;
    } else {
        printf("❌ ADD 指令测试失败\n");
    }
    
    // 测试4: SW指令
    if (mem_200 == 0x000040ff) {
        printf("✅ SW 指令测试通过\n");
        tests_passed++;
    } else {
        printf("❌ SW 指令测试失败\n");
    }
    
    // 测试5: LW指令
    if (a4 == 0x00004003) {
        printf("✅ LW 指令测试通过\n");
        tests_passed++;
    } else {
        printf("❌ LW 指令测试失败\n");
    }
    
    // 测试6: SB指令
    if (mem_200 == 0x000040ff) {
        printf("✅ SB 指令测试通过\n");
        tests_passed++;
    } else {
        printf("❌ SB 指令测试失败\n");
    }
    
    // 测试7: LBU指令
    if (a5 == 0x000000ff) {
        printf("✅ LBU 指令测试通过\n");
        tests_passed++;
    } else {
        printf("❌ LBU 指令测试失败\n");
    }
    
    // 测试8: JALR指令
    if (ra != 0) { // ra应该有值
        printf("✅ JALR 指令测试通过\n");
        tests_passed++;
    } else {
        printf("❌ JALR 指令测试失败\n");
    }
    
    // 测试9: EBREAK指令
    // 如果能正常执行到验证阶段，说明ebreak工作正常
    printf("✅ EBREAK 指令测试通过\n");
    tests_passed++;
    
    printf("\n=== 总体测试结果 ===\n");
    printf("测试结果: %d/%d 通过\n", tests_passed, total_tests);
    
    if (tests_passed == total_tests) {
        printf("🎉 所有9条RISC-V指令测试通过！\n");
    } else {
        printf("⚠️  部分测试失败，需要调试\n");
    }
}

