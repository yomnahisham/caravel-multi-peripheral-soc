# Task List

1. ✅ Set up Caravel user project structure and documentation
Template copied, docs created: README.md, register_map.md, pad_map.md, integration_notes.md
2. ✅ Link required IP cores using ipm_linker
Successfully linked CF_SPI (v2.0.1), EF_I2C (v1.1.0), EF_GPIO8 (v1.1.0) with dependencies
3. ✅ Develop user_project with Wishbone address decoding
Created user_project.v with WB decoder using bits [17:16] for 4 peripherals
4. ✅ Create user_project_wrapper module
Updated wrapper to instantiate user_project and connect to Caravel pads
5. 🔄 Create cocotb verification tests
Individual tests for each peripheral (SPI0, SPI1, I2C, GPIO) plus system integration test
6. ⏳ Run caravel-cocotb tests for RTL verification
Execute all tests and ensure they pass before PnR
7. ⏳ Write OpenLane configuration files
Config for user_project and user_project_wrapper
8. ⏳ Harden user_project macro using OpenLane
Run OpenLane flow for user_project
9. ⏳ Harden user_project_wrapper using OpenLane
Run OpenLane flow for wrapper with hardened user_project
10. ⏳ Generate final documentation and retrospective
Complete README, retrospective.md with results and acceptance checklist

