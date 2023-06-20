@echo off

F:\GameDev\Vulkan\SDK\Bin\glslc.exe shader.vert -o shader.vert.spv
F:\GameDev\Vulkan\SDK\Bin\glslc.exe shader.frag -o shader.frag.spv
F:\GameDev\Vulkan\SDK\Bin\glslc.exe pointLight.vert -o pointLight.vert.spv
F:\GameDev\Vulkan\SDK\Bin\glslc.exe pointLight.frag -o pointLight.frag.spv
pause