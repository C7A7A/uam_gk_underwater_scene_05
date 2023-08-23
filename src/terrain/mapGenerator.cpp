#include "mapGenerator.h"

void createHeightMap(float noiseWidth, float noiseHeight, float vertWidth, float vertHeight) {
	// Produces 3D ridged multifractal noise, similar to mountains
	module::RidgedMulti baseMountainTerrain;

	module::Spheres baseFlatTerrain;
	baseFlatTerrain.SetFrequency(randNumberDouble(1.0, 4.0));

	//if (baseTerrainType == 0) {
	//	// Produces polygon-like formations
	//	module::Voronoi baseFlatTerrain;

	//	// The frequency determines the size of the Voronoi cells and the
	//	// distance between these cells.
	//	baseFlatTerrain.SetFrequency(randNumberDouble(1.0, 3.0));

	//	// Scale of the random displacement to apply to each Voronoi cell.	
	//	baseFlatTerrain.SetDisplacement(randNumberDouble(0.1, 0.25));
	//} else if (baseTerrainType == 1) {
	//	// Generates "Billowy" noise suitable for clouds and rocks?
	//	module::Billow baseFlatTerrain;
	//	baseFlatTerrain.SetFrequency(randNumberDouble(2.0, 5.0));
	//} else if (baseTerrainType == 2) {
	//	module::Spheres baseFlatTerrain;
	//	baseFlatTerrain.SetFrequency(randNumberDouble(1.0, 3.0));
	//}

	// Applies a scaling factor to the output value from the source module
	// Scales the flat terrain, adds noise to it
	module::ScaleBias flatTerrain;
	flatTerrain.SetSourceModule(0, baseFlatTerrain);
	flatTerrain.SetScale(randNumberDouble(0.8, 1));
	flatTerrain.SetBias(randNumberDouble(0, 0.2));

	module::Perlin terrainType;

	module::Select terrainSelector;
	terrainSelector.SetSourceModule(0, flatTerrain);
	terrainSelector.SetSourceModule(1, baseMountainTerrain);
	terrainSelector.SetControlModule(terrainType);
	terrainSelector.SetBounds(0.0, 1000); //1000
	terrainSelector.SetEdgeFalloff(randNumberDouble(0.1, 0.3)); // .125

	// pseudo-random displacement of the input value
	module::Turbulence finalTerrain;
	finalTerrain.SetSourceModule(0, terrainSelector);
	finalTerrain.SetFrequency(randNumberDouble(1.0, 3.0)); // How rapidly the displacement changes
	finalTerrain.SetPower(randNumberDouble(0.1, 0.4)); // The scaling factor that is applied to the displacement amount

	// Output the noise map
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(finalTerrain);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(noiseWidth, noiseHeight);
	heightMapBuilder.SetBounds(0, vertWidth, 0, vertHeight);
	heightMapBuilder.Build();

	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.Render();

	utils::WriterBMP writer;
	writer.SetSourceImage(image);
	writer.SetDestFilename("textures/heightmaps/heightmap.bmp");
	writer.WriteDestFile();
}