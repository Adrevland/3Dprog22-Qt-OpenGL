#pragma once
#include <chrono>
#include <vector>

class Heightmap;
class Mesh;

class Spawner
{
public:
	Spawner(Mesh* owner, const char* obj, const char* texture, Heightmap* hmap);
	~Spawner();
	void CreateObjectToSpawn(const char* obj, const char* texture, Heightmap* hmap);
	void spawn(float spawnintervall);
	void destroy(Mesh* mesh);
	void draw();
	bool checkoverlapp(Mesh* mesh);

	void simbomb();

private:
	const char* mObj;
	const char* mTexture;
	Heightmap* mHmap{nullptr};

	Mesh* mOwner{nullptr};
	std::vector<Mesh*> mSpawnedMeshes;
	std::chrono::steady_clock::time_point lastspawn;
};