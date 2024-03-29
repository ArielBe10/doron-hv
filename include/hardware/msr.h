#pragma once


#define MSR_IA32_P5_MC_ADDR                  0x00000000U
#define MSR_IA32_P5_MC_TYPE                  0x00000001U
#define MSR_IA32_MONITOR_FILTER_SIZE         0x00000006U
#define MSR_IA32_TIME_STAMP_COUNTER          0x00000010U
#define MSR_IA32_PLATFORM_ID                 0x00000017U
#define MSR_IA32_APIC_BASE                   0x0000001bU
#define MSR_IA32_FEATURE_CONTROL             0x0000003aU
#define MSR_IA32_TSC_ADJUST                  0x0000003bU
#define MSR_IA32_BIOS_UPDT_TRIG              0x00000079U
#define MSR_IA32_BIOS_SIGN_ID                0x0000008bU
#define MSR_IA32_SGXLEPUBKEYHASH0            0x0000008cU
#define MSR_IA32_SGXLEPUBKEYHASH1            0x0000008dU
#define MSR_IA32_SGXLEPUBKEYHASH2            0x0000008eU
#define MSR_IA32_SGXLEPUBKEYHASH3            0x0000008fU
#define MSR_IA32_SMM_MONITOR_CTL             0x0000009bU
#define MSR_IA32_SMBASE                      0x0000009eU
#define MSR_IA32_PMC0                        0x000000c1U
#define MSR_IA32_PMC1                        0x000000c2U
#define MSR_IA32_PMC2                        0x000000c3U
#define MSR_IA32_PMC3                        0x000000c4U
#define MSR_IA32_PMC4                        0x000000c5U
#define MSR_IA32_PMC5                        0x000000c6U
#define MSR_IA32_PMC6                        0x000000c7U
#define MSR_IA32_PMC7                        0x000000c8U
#define MSR_IA32_MPERF                       0x000000e7U
#define MSR_IA32_APERF                       0x000000e8U
#define MSR_IA32_MTRRCAP                     0x000000feU
#define MSR_IA32_SYSENTER_CS                 0x00000174U
#define MSR_IA32_SYSENTER_ESP                0x00000175U
#define MSR_IA32_SYSENTER_EIP                0x00000176U
#define MSR_IA32_MCG_CAP                     0x00000179U
#define MSR_IA32_MCG_STATUS                  0x0000017aU
#define MSR_IA32_MCG_CTL                     0x0000017bU
#define MSR_IA32_PERFEVTSEL0                 0x00000186U
#define MSR_IA32_PERFEVTSEL1                 0x00000187U
#define MSR_IA32_PERFEVTSEL2                 0x00000188U
#define MSR_IA32_PERFEVTSEL3                 0x00000189U
#define MSR_IA32_PERF_STATUS                 0x00000198U
#define MSR_IA32_PERF_CTL                    0x00000199U
#define MSR_IA32_CLOCK_MODULATION            0x0000019aU
#define MSR_IA32_THERM_INTERRUPT             0x0000019bU
#define MSR_IA32_THERM_STATUS                0x0000019cU
#define MSR_IA32_MISC_ENABLE                 0x000001a0U
#define MSR_IA32_ENERGY_PERF_BIAS            0x000001b0U
#define MSR_IA32_PACKAGE_THERM_STATUS        0x000001b1U
#define MSR_IA32_PACKAGE_THERM_INTERRUPT     0x000001b2U
#define MSR_IA32_DEBUGCTL                    0x000001d9U
#define MSR_IA32_SMRR_PHYSBASE               0x000001f2U
#define MSR_IA32_SMRR_PHYSMASK               0x000001f3U
#define MSR_IA32_PLATFORM_DCA_CAP            0x000001f8U
#define MSR_IA32_CPU_DCA_CAP                 0x000001f9U
#define MSR_IA32_DCA_0_CAP                   0x000001faU
#define MSR_IA32_MTRR_PHYSBASE0              0x00000200U
#define MSR_IA32_MTRR_PHYSMASK0              0x00000201U
#define MSR_IA32_MTRR_PHYSBASE1              0x00000202U
#define MSR_IA32_MTRR_PHYSMASK1              0x00000203U
#define MSR_IA32_MTRR_PHYSBASE2              0x00000204U
#define MSR_IA32_MTRR_PHYSMASK2              0x00000205U
#define MSR_IA32_MTRR_PHYSBASE3              0x00000206U
#define MSR_IA32_MTRR_PHYSMASK3              0x00000207U
#define MSR_IA32_MTRR_PHYSBASE4              0x00000208U
#define MSR_IA32_MTRR_PHYSMASK4              0x00000209U
#define MSR_IA32_MTRR_PHYSBASE5              0x0000020aU
#define MSR_IA32_MTRR_PHYSMASK5              0x0000020bU
#define MSR_IA32_MTRR_PHYSBASE6              0x0000020cU
#define MSR_IA32_MTRR_PHYSMASK6              0x0000020dU
#define MSR_IA32_MTRR_PHYSBASE7              0x0000020eU
#define MSR_IA32_MTRR_PHYSMASK7              0x0000020fU
#define MSR_IA32_MTRR_PHYSBASE8              0x00000210U
#define MSR_IA32_MTRR_PHYSMASK8              0x00000211U
#define MSR_IA32_MTRR_PHYSBASE9              0x00000212U
#define MSR_IA32_MTRR_PHYSMASK9              0x00000213U
#define MSR_IA32_MTRR_FIX64K_00000           0x00000250U
#define MSR_IA32_MTRR_FIX16K_80000           0x00000258U
#define MSR_IA32_MTRR_FIX16K_A0000           0x00000259U
#define MSR_IA32_MTRR_FIX4K_C0000            0x00000268U
#define MSR_IA32_MTRR_FIX4K_C8000            0x00000269U
#define MSR_IA32_MTRR_FIX4K_D0000            0x0000026aU
#define MSR_IA32_MTRR_FIX4K_D8000            0x0000026bU
#define MSR_IA32_MTRR_FIX4K_E0000            0x0000026cU
#define MSR_IA32_MTRR_FIX4K_E8000            0x0000026dU
#define MSR_IA32_MTRR_FIX4K_F0000            0x0000026eU
#define MSR_IA32_MTRR_FIX4K_F8000            0x0000026fU
#define MSR_IA32_PAT                         0x00000277U
#define MSR_IA32_MC0_CTL2                    0x00000280U
#define MSR_IA32_MC1_CTL2                    0x00000281U
#define MSR_IA32_MC2_CTL2                    0x00000282U
#define MSR_IA32_MC3_CTL2                    0x00000283U
#define MSR_IA32_MC4_CTL2                    0x00000284U
#define MSR_IA32_MC5_CTL2                    0x00000285U
#define MSR_IA32_MC6_CTL2                    0x00000286U
#define MSR_IA32_MC7_CTL2                    0x00000287U
#define MSR_IA32_MC8_CTL2                    0x00000288U
#define MSR_IA32_MC9_CTL2                    0x00000289U
#define MSR_IA32_MC10_CTL2                   0x0000028aU
#define MSR_IA32_MC11_CTL2                   0x0000028bU
#define MSR_IA32_MC12_CTL2                   0x0000028cU
#define MSR_IA32_MC13_CTL2                   0x0000028dU
#define MSR_IA32_MC14_CTL2                   0x0000028eU
#define MSR_IA32_MC15_CTL2                   0x0000028fU
#define MSR_IA32_MC16_CTL2                   0x00000290U
#define MSR_IA32_MC17_CTL2                   0x00000291U
#define MSR_IA32_MC18_CTL2                   0x00000292U
#define MSR_IA32_MC19_CTL2                   0x00000293U
#define MSR_IA32_MC20_CTL2                   0x00000294U
#define MSR_IA32_MC21_CTL2                   0x00000295U
#define MSR_IA32_MC22_CTL2                   0x00000296U
#define MSR_IA32_MC23_CTL2                   0x00000297U
#define MSR_IA32_MC24_CTL2                   0x00000298U
#define MSR_IA32_MC25_CTL2                   0x00000299U
#define MSR_IA32_MC26_CTL2                   0x0000029aU
#define MSR_IA32_MC27_CTL2                   0x0000029bU
#define MSR_IA32_MC28_CTL2                   0x0000029cU
#define MSR_IA32_MC29_CTL2                   0x0000029dU
#define MSR_IA32_MC30_CTL2                   0x0000029eU
#define MSR_IA32_MC31_CTL2                   0x0000029fU
#define MSR_IA32_MTRR_DEF_TYPE               0x000002ffU
#define MSR_IA32_FIXED_CTR0                  0x00000309U
#define MSR_IA32_FIXED_CTR1                  0x0000030aU
#define MSR_IA32_FIXED_CTR2                  0x0000030bU
#define MSR_IA32_PERF_CAPABILITIES           0x00000345U
#define MSR_IA32_FIXED_CTR_CTRL              0x0000038dU
#define MSR_IA32_PERF_GLOBAL_STATUS          0x0000038eU
#define MSR_IA32_PERF_GLOBAL_CTRL            0x0000038fU
#define MSR_IA32_PERF_GLOBAL_OVF_CTRL        0x00000390U
#define MSR_IA32_PERF_GLOBAL_STATUS_RESET    0x00000390U
#define MSR_IA32_PERF_GLOBAL_STATUS_SET      0x00000391U
#define MSR_IA32_PERF_GLOBAL_INUSE           0x00000392U
#define MSR_IA32_PEBS_ENABLE                 0x000003f1U
#define MSR_IA32_MC0_CTL                     0x00000400U
#define MSR_IA32_MC0_STATUS                  0x00000401U
#define MSR_IA32_MC0_ADDR                    0x00000402U
#define MSR_IA32_MC0_MISC                    0x00000403U
#define MSR_IA32_MC1_CTL                     0x00000404U
#define MSR_IA32_MC1_STATUS                  0x00000405U
#define MSR_IA32_MC1_ADDR                    0x00000406U
#define MSR_IA32_MC1_MISC                    0x00000407U
#define MSR_IA32_MC2_CTL                     0x00000408U
#define MSR_IA32_MC2_STATUS                  0x00000409U
#define MSR_IA32_MC2_ADDR                    0x0000040aU
#define MSR_IA32_MC2_MISC                    0x0000040bU
#define MSR_IA32_MC3_CTL                     0x0000040cU
#define MSR_IA32_MC3_STATUS                  0x0000040dU
#define MSR_IA32_MC3_ADDR                    0x0000040eU
#define MSR_IA32_MC3_MISC                    0x0000040fU
#define MSR_IA32_MC4_CTL                     0x00000410U
#define MSR_IA32_MC4_STATUS                  0x00000411U
#define MSR_IA32_MC4_ADDR                    0x00000412U
#define MSR_IA32_MC4_MISC                    0x00000413U
#define MSR_IA32_MC5_CTL                     0x00000414U
#define MSR_IA32_MC5_STATUS                  0x00000415U
#define MSR_IA32_MC5_ADDR                    0x00000416U
#define MSR_IA32_MC5_MISC                    0x00000417U
#define MSR_IA32_MC6_CTL                     0x00000418U
#define MSR_IA32_MC6_STATUS                  0x00000419U
#define MSR_IA32_MC6_ADDR                    0x0000041aU
#define MSR_IA32_MC6_MISC                    0x0000041bU
#define MSR_IA32_MC7_CTL                     0x0000041cU
#define MSR_IA32_MC7_STATUS                  0x0000041dU
#define MSR_IA32_MC7_ADDR                    0x0000041eU
#define MSR_IA32_MC7_MISC                    0x0000041fU
#define MSR_IA32_MC8_CTL                     0x00000420U
#define MSR_IA32_MC8_STATUS                  0x00000421U
#define MSR_IA32_MC8_ADDR                    0x00000422U
#define MSR_IA32_MC8_MISC                    0x00000423U
#define MSR_IA32_MC9_CTL                     0x00000424U
#define MSR_IA32_MC9_STATUS                  0x00000425U
#define MSR_IA32_MC9_ADDR                    0x00000426U
#define MSR_IA32_MC9_MISC                    0x00000427U
#define MSR_IA32_MC10_CTL                    0x00000428U
#define MSR_IA32_MC10_STATUS                 0x00000429U
#define MSR_IA32_MC10_ADDR                   0x0000042aU
#define MSR_IA32_MC10_MISC                   0x0000042bU
#define MSR_IA32_MC11_CTL                    0x0000042cU
#define MSR_IA32_MC11_STATUS                 0x0000042dU
#define MSR_IA32_MC11_ADDR                   0x0000042eU
#define MSR_IA32_MC11_MISC                   0x0000042fU
#define MSR_IA32_MC12_CTL                    0x00000430U
#define MSR_IA32_MC12_STATUS                 0x00000431U
#define MSR_IA32_MC12_ADDR                   0x00000432U
#define MSR_IA32_MC12_MISC                   0x00000433U
#define MSR_IA32_MC13_CTL                    0x00000434U
#define MSR_IA32_MC13_STATUS                 0x00000435U
#define MSR_IA32_MC13_ADDR                   0x00000436U
#define MSR_IA32_MC13_MISC                   0x00000437U
#define MSR_IA32_MC14_CTL                    0x00000438U
#define MSR_IA32_MC14_STATUS                 0x00000439U
#define MSR_IA32_MC14_ADDR                   0x0000043aU
#define MSR_IA32_MC14_MISC                   0x0000043bU
#define MSR_IA32_MC15_CTL                    0x0000043cU
#define MSR_IA32_MC15_STATUS                 0x0000043dU
#define MSR_IA32_MC15_ADDR                   0x0000043eU
#define MSR_IA32_MC15_MISC                   0x0000043fU
#define MSR_IA32_MC16_CTL                    0x00000440U
#define MSR_IA32_MC16_STATUS                 0x00000441U
#define MSR_IA32_MC16_ADDR                   0x00000442U
#define MSR_IA32_MC16_MISC                   0x00000443U
#define MSR_IA32_MC17_CTL                    0x00000444U
#define MSR_IA32_MC17_STATUS                 0x00000445U
#define MSR_IA32_MC17_ADDR                   0x00000446U
#define MSR_IA32_MC17_MISC                   0x00000447U
#define MSR_IA32_MC18_CTL                    0x00000448U
#define MSR_IA32_MC18_STATUS                 0x00000449U
#define MSR_IA32_MC18_ADDR                   0x0000044aU
#define MSR_IA32_MC18_MISC                   0x0000044bU
#define MSR_IA32_MC19_CTL                    0x0000044cU
#define MSR_IA32_MC19_STATUS                 0x0000044dU
#define MSR_IA32_MC19_ADDR                   0x0000044eU
#define MSR_IA32_MC19_MISC                   0x0000044fU
#define MSR_IA32_MC20_CTL                    0x00000450U
#define MSR_IA32_MC20_STATUS                 0x00000451U
#define MSR_IA32_MC20_ADDR                   0x00000452U
#define MSR_IA32_MC20_MISC                   0x00000453U
#define MSR_IA32_MC21_CTL                    0x00000454U
#define MSR_IA32_MC21_STATUS                 0x00000455U
#define MSR_IA32_MC21_ADDR                   0x00000456U
#define MSR_IA32_MC21_MISC                   0x00000457U
#define MSR_IA32_MC22_CTL                    0x00000458U
#define MSR_IA32_MC22_STATUS                 0x00000459U
#define MSR_IA32_MC22_ADDR                   0x0000045aU
#define MSR_IA32_MC22_MISC                   0x0000045bU
#define MSR_IA32_MC23_CTL                    0x0000045cU
#define MSR_IA32_MC23_STATUS                 0x0000045dU
#define MSR_IA32_MC23_ADDR                   0x0000045eU
#define MSR_IA32_MC23_MISC                   0x0000045fU
#define MSR_IA32_MC24_CTL                    0x00000460U
#define MSR_IA32_MC24_STATUS                 0x00000461U
#define MSR_IA32_MC24_ADDR                   0x00000462U
#define MSR_IA32_MC24_MISC                   0x00000463U
#define MSR_IA32_MC25_CTL                    0x00000464U
#define MSR_IA32_MC25_STATUS                 0x00000465U
#define MSR_IA32_MC25_ADDR                   0x00000466U
#define MSR_IA32_MC25_MISC                   0x00000467U
#define MSR_IA32_MC26_CTL                    0x00000468U
#define MSR_IA32_MC26_STATUS                 0x00000469U
#define MSR_IA32_MC26_ADDR                   0x0000046aU
#define MSR_IA32_MC26_MISC                   0x0000046bU
#define MSR_IA32_MC27_CTL                    0x0000046cU
#define MSR_IA32_MC27_STATUS                 0x0000046dU
#define MSR_IA32_MC27_ADDR                   0x0000046eU
#define MSR_IA32_MC27_MISC                   0x0000046fU
#define MSR_IA32_MC28_CTL                    0x00000470U
#define MSR_IA32_MC28_STATUS                 0x00000471U
#define MSR_IA32_MC28_ADDR                   0x00000472U
#define MSR_IA32_MC28_MISC                   0x00000473U
#define MSR_IA32_VMX_BASIC                   0x00000480U
#define MSR_IA32_VMX_PINBASED_CTLS           0x00000481U
#define MSR_IA32_VMX_PROCBASED_CTLS          0x00000482U
#define MSR_IA32_VMX_EXIT_CTLS               0x00000483U
#define MSR_IA32_VMX_ENTRY_CTLS              0x00000484U
#define MSR_IA32_VMX_MISC                    0x00000485U
#define MSR_IA32_VMX_CR0_FIXED0              0x00000486U
#define MSR_IA32_VMX_CR0_FIXED1              0x00000487U
#define MSR_IA32_VMX_CR4_FIXED0              0x00000488U
#define MSR_IA32_VMX_CR4_FIXED1              0x00000489U
#define MSR_IA32_VMX_VMCS_ENUM               0x0000048aU
#define MSR_IA32_VMX_PROCBASED_CTLS2         0x0000048bU
#define MSR_IA32_VMX_EPT_VPID_CAP            0x0000048cU
#define MSR_IA32_VMX_TRUE_PINBASED_CTLS      0x0000048dU
#define MSR_IA32_VMX_TRUE_PROCBASED_CTLS     0x0000048eU
#define MSR_IA32_VMX_TRUE_EXIT_CTLS          0x0000048fU
#define MSR_IA32_VMX_TRUE_ENTRY_CTLS         0x00000490U
#define MSR_IA32_VMX_VMFUNC                  0x00000491U
#define MSR_IA32_A_PMC0                      0x000004c1U
#define MSR_IA32_A_PMC1                      0x000004c2U
#define MSR_IA32_A_PMC2                      0x000004c3U
#define MSR_IA32_A_PMC3                      0x000004c4U
#define MSR_IA32_A_PMC4                      0x000004c5U
#define MSR_IA32_A_PMC5                      0x000004c6U
#define MSR_IA32_A_PMC6                      0x000004c7U
#define MSR_IA32_A_PMC7                      0x000004c8U
#define MSR_IA32_MCG_EXT_CTL                 0x000004d0U
#define MSR_IA32_SGX_SVN_STATUS              0x00000500U
#define MSR_IA32_RTIT_OUTPUT_BASE            0x00000560U
#define MSR_IA32_RTIT_OUTPUT_MASK_PTRS       0x00000561U
#define MSR_IA32_RTIT_CTL                    0x00000570U
#define MSR_IA32_RTIT_STATUS                 0x00000571U
#define MSR_IA32_RTIT_CR3_MATCH              0x00000572U
#define MSR_IA32_RTIT_ADDR0_A                0x00000580U
#define MSR_IA32_RTIT_ADDR0_B                0x00000581U
#define MSR_IA32_RTIT_ADDR1_A                0x00000582U
#define MSR_IA32_RTIT_ADDR1_B                0x00000583U
#define MSR_IA32_RTIT_ADDR2_A                0x00000584U
#define MSR_IA32_RTIT_ADDR2_B                0x00000585U
#define MSR_IA32_RTIT_ADDR3_A                0x00000586U
#define MSR_IA32_RTIT_ADDR3_B                0x00000587U
#define MSR_IA32_DS_AREA                     0x00000600U
#define MSR_IA32_TSC_DEADLINE                0x000006e0U
#define MSR_IA32_PM_ENABLE                   0x00000770U
#define MSR_IA32_HWP_CAPABILITIES            0x00000771U
#define MSR_IA32_HWP_REQUEST_PKG             0x00000772U
#define MSR_IA32_HWP_INTERRUPT               0x00000773U
#define MSR_IA32_HWP_REQUEST                 0x00000774U
#define MSR_IA32_HWP_STATUS                  0x00000777U
#define MSR_IA32_X2APIC_APICID               0x00000802U
#define MSR_IA32_X2APIC_VERSION              0x00000803U
#define MSR_IA32_X2APIC_TPR                  0x00000808U
#define MSR_IA32_X2APIC_PPR                  0x0000080aU
#define MSR_IA32_X2APIC_EOI                  0x0000080bU
#define MSR_IA32_X2APIC_LDR                  0x0000080dU
#define MSR_IA32_X2APIC_SIVR                 0x0000080fU
#define MSR_IA32_X2APIC_ISR0                 0x00000810U
#define MSR_IA32_X2APIC_ISR1                 0x00000811U
#define MSR_IA32_X2APIC_ISR2                 0x00000812U
#define MSR_IA32_X2APIC_ISR3                 0x00000813U
#define MSR_IA32_X2APIC_ISR4                 0x00000814U
#define MSR_IA32_X2APIC_ISR5                 0x00000815U
#define MSR_IA32_X2APIC_ISR6                 0x00000816U
#define MSR_IA32_X2APIC_ISR7                 0x00000817U
#define MSR_IA32_X2APIC_TMR0                 0x00000818U
#define MSR_IA32_X2APIC_TMR1                 0x00000819U
#define MSR_IA32_X2APIC_TMR2                 0x0000081aU
#define MSR_IA32_X2APIC_TMR3                 0x0000081bU
#define MSR_IA32_X2APIC_TMR4                 0x0000081cU
#define MSR_IA32_X2APIC_TMR5                 0x0000081dU
#define MSR_IA32_X2APIC_TMR6                 0x0000081eU
#define MSR_IA32_X2APIC_TMR7                 0x0000081fU
#define MSR_IA32_X2APIC_IRR0                 0x00000820U
#define MSR_IA32_X2APIC_IRR1                 0x00000821U
#define MSR_IA32_X2APIC_IRR2                 0x00000822U
#define MSR_IA32_X2APIC_IRR3                 0x00000823U
#define MSR_IA32_X2APIC_IRR4                 0x00000824U
#define MSR_IA32_X2APIC_IRR5                 0x00000825U
#define MSR_IA32_X2APIC_IRR6                 0x00000826U
#define MSR_IA32_X2APIC_IRR7                 0x00000827U
#define MSR_IA32_X2APIC_ESR                  0x00000828U
#define MSR_IA32_X2APIC_LVT_CMCI             0x0000082fU
#define MSR_IA32_X2APIC_ICR                  0x00000830U
#define MSR_IA32_X2APIC_LVT_TIMER            0x00000832U
#define MSR_IA32_X2APIC_LVT_THERMAL          0x00000833U
#define MSR_IA32_X2APIC_LVT_PMI              0x00000834U
#define MSR_IA32_X2APIC_LVT_LINT0            0x00000835U
#define MSR_IA32_X2APIC_LVT_LINT1            0x00000836U
#define MSR_IA32_X2APIC_LVT_ERROR            0x00000837U
#define MSR_IA32_X2APIC_INIT_COUNT           0x00000838U
#define MSR_IA32_X2APIC_CUR_COUNT            0x00000839U
#define MSR_IA32_X2APIC_DIV_CONF             0x0000083eU
#define MSR_IA32_X2APIC_SELF_IPI             0x0000083fU
#define MSR_IA32_DEBUG_INTERFACE             0x00000c80U
#define MSR_IA32_L3_QOS_CFG                  0x00000c81U
#define MSR_IA32_QM_EVTSEL                   0x00000c8dU
#define MSR_IA32_QM_CTR                      0x00000c8eU
#define MSR_IA32_PQR_ASSOC                   0x00000c8fU
#define MSR_IA32_L3_MASK_0                   0x00000c90U
#define MSR_IA32_L2_MASK_0                   0x00000d10U
#define MSR_IA32_BNDCFGS                     0x00000d90U
#define MSR_IA32_XSS                         0x00000da0U
#define MSR_IA32_PKG_HDC_CTL                 0x00000db0U
#define MSR_IA32_PM_CTL1                     0x00000db1U
#define MSR_IA32_THREAD_STALL                0x00000db2U
#define MSR_IA32_EFER                        0xc0000080U
#define MSR_IA32_STAR                        0xc0000081U
#define MSR_IA32_LSTAR                       0xc0000082U
#define MSR_IA32_FMASK                       0xc0000084U
#define MSR_IA32_FS_BASE                     0xc0000100U
#define MSR_IA32_GS_BASE                     0xc0000101U
#define MSR_IA32_KERNEL_GS_BASE              0xc0000102U
#define MSR_IA32_TSC_AUX                     0xc0000103U