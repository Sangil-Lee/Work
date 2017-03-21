open_project {pa_proj.ppr}
# import_files -force -norecurse {..\src\crc.v}
# set_property library work [get_files -of_objects [get_property srcset [current_run]] {pa_proj.srcs\sources_1\imports\crc.v}]
# add_files -fileset [get_property constrset [current_run]] -norecurse {..\ucf\crc.ucf}
# set_property target_ucf {..\ucf\crc.ucf} [get_property constrset [current_run]]
set_property add_step Bitgen [get_runs impl_1]
launch_runs -runs impl_1 -jobs 1 -dir {pa_proj.runs}
