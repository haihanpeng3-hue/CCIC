set project_name Loongson_Soc_validate
set project_path ./project_validate
set project_part xc7a200tfbg676-1

file delete -force $project_path
create_project -force $project_name $project_path -part $project_part
add_files -scan_for_includes ../rtl
add_files -norecurse -scan_for_includes ../rtl/ip/PLL_2019_2/clk_pll.xci
add_files -fileset sim_1 ../sim/
add_files -fileset constrs_1 -quiet ./constraints
set_property top soc_top [current_fileset]
set_property -name top -value tb_top -objects [get_filesets sim_1]
close_project
