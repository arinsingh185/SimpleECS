#include<iostream>
#include <chrono>
#include <immintrin.h>

struct Positon {
	float x, y;
};

struct Velocity{
	float dx, dy;
};

struct ArchetypeChunk {
	float* posX, * posY, * velX, * velY;
	size_t entityCount; 

	ArchetypeChunk(size_t size) : entityCount(size) {
		posX = (float*)_aligned_malloc(size * sizeof(float), 64);
		posY = (float*)_aligned_malloc(size * sizeof(float), 64);
		velX = (float*)_aligned_malloc(size * sizeof(float), 64);
		velY = (float*)_aligned_malloc(size * sizeof(float), 64);
	}

	~ArchetypeChunk() {
		_aligned_free(posX); _aligned_free(posY);
		_aligned_free(velX); _aligned_free(velY);
	}
};

void InitializeChunk(ArchetypeChunk& chunk) {
	for (size_t i = 0; i < chunk.entityCount; ++i) {
		chunk.posX[i] = static_cast<float>(i);
		chunk.posY[i] = static_cast<float>(i);
		chunk.velX[i] = 1.0f;
		chunk.velY[i] = 1.0f;
	}
}

void MovementSystem(ArchetypeChunk& chunk) {

	for (size_t i = 0; i < chunk.entityCount; ++i) {
		chunk.posX[i] += chunk.velX[i];
		chunk.posY[i] += chunk.velY[i];
	}
}



void MovementSystemSIMD(ArchetypeChunk& chunk) {
	
	for (size_t i = 0; i < chunk.entityCount; i += 8) {

	
		__m256 posX_vec = _mm256_load_ps(&chunk.posX[i]);
		__m256 velX_vec = _mm256_load_ps(&chunk.velX[i]);

		__m256 resultX = _mm256_add_ps(posX_vec, velX_vec);

		_mm256_store_ps(&chunk.posX[i], resultX);

		__m256 posY_vec = _mm256_load_ps(&chunk.posY[i]);
		__m256 velY_vec = _mm256_load_ps(&chunk.velY[i]);

		__m256 resultY = _mm256_add_ps(posY_vec, velY_vec);

		_mm256_store_ps(&chunk.posY[i], resultY);
	}
}

int main() {
	const size_t numEntities = 1000000;


	auto myChunk = std::make_unique<ArchetypeChunk>(numEntities);

	InitializeChunk(*myChunk);

	auto start = std::chrono::high_resolution_clock::now();

	for (int frame = 0; frame < 1000; ++frame) {
		MovementSystemSIMD(*myChunk);
	}

	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double, std::milli> elapsed = end - start;


	std::cout << "Last Pos X: " << myChunk->posX[numEntities - 1] << std::endl;
	std::cout << "Time: " << elapsed.count() << " ms" << std::endl;

	return 0;
}