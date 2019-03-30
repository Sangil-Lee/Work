# RAON MPS OPI displays

## Files

The main display is fps_overview.opi. This file has two macros that need to match the IOC: `$(P)` and `$(R)`.

All files beginning with the underscore (`_`) character are fragments that are integrated into the top-level displays.

### OPI files

The `_<filename>.opi` files are accessed from linking containers in the main display.

The following files are not automatically generated:

- `fps_engineering.opi`
- `fps_fpga_link_status.opi`
- `fps_fpga_mitigation_node_status.opi`

### XML files

The `_<filename>.xml` files are used by the Python scripts to support the generation of the main displays.

The XML files contain macros in the form `\$macro` that are substituted with appropriate values by the Python scripts.

### Python files

Python scripts generate two of the OPI files:

- `fps_overview_gen.py` generates `fps_overview.opi`
- `fps_output_detail_gen.py` generates `fps_output_detail.opi`

### BOY files

The `cfg` directory contains font and color definition files for BOY.

### Externally provided files

The `extern` directory contains files imported from the synApps package.


