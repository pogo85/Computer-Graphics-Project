/**
	* File:    vboindexer.cpp
	*
	* Author1:  Akshay Tiwari (2018A7PS0201H)
	* Author2:  Mushkan Surekha  (2018B5A70477H)
	* Author3:  Mahesh Swaminathan  (2018A4PS0982H)
	* Date:     09-04-2020
	* Course:   Computer Graphics (IS F311)
	*
	* Summary of File:
	*
	*   Program to push indices into index array by a quick alorithm.
	*	Also pushes vertices, uvs and normals into respective arrays.
*/
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include "vboindexer.hpp"
#include <string.h>

struct PackedVertex {
	glm::vec3 position;///<Pointer to left subtree of node
	glm::vec2 uv;///<Pointer to left subtree of node
	glm::vec3 normal;///<Pointer to left subtree of node
	bool operator<(const PackedVertex that) const {
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
	};
};

/**
	*   bool getSimilarVertexIndex(PackedVertex& packed, std::map<PackedVertex, unsigned short>& VertexToOutIndex, unsigned short& result)
	*
	*   Summary of getSimilarVertexIndex function:
	*
	*       Checks if current vertex is already present in index array(buffer)
	*
	*   Parameters  :
	*
	*		PackedVertex& packed - Struct with vertex, uv and normal data
	*		std::map<PackedVertex, unsigned short>& VertexToOutIndex - Looks for similar vertex in this map
	*		unsigned short& result - Stores index if similar vertex found
	*
	*   Return Value :
	*
	*		Boolean - True if similar vertex found; False otherwise
	*
	*   Description :
	*
	*       Checks if current vertex is already present in index buffer.
	*		If present, stores index in result and returns true.
	*		Uses the created struct for faster calculation than linear search.
*/
bool getSimilarVertexIndex(
	PackedVertex& packed,
	std::map<PackedVertex, unsigned short>& VertexToOutIndex,
	unsigned short& result
	) {
	std::map<PackedVertex, unsigned short>::iterator it = VertexToOutIndex.find(packed);
	if (it == VertexToOutIndex.end()) {
		return false;
	}
	else {
		result = it->second;
		return true;
	}
}
/**
	*   void indexVBO(std::vector<glm::vec3>& in_vertices, std::vector<glm::vec2>& in_uvs, std::vector<glm::vec3>& in_normals, std::vector<unsigned short>& out_indices, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals)
	*
	*   Summary of indexVBO function:
	*
	*       Indexes input vertex, uv and normal vector into output vertex, uv and normal arrays after storing near indices in an index array
	*
	*   Parameters  :
	*
	*		std::vector<glm::vec3>& in_vertices - Input vertices vector
	*		std::vector<glm::vec2>& in_uvs - Input uvs vector
	*		std::vector<glm::vec3>& in_normals - Input normals vector
	*		std::vector<unsigned short>& out_indices - Output indices vector
	*		std::vector<glm::vec3>& out_vertices - Output vertices vector
	*		std::vector<glm::vec2>& out_uvs - Output uvs vector
	*		std::vector<glm::vec3>& out_normals - Output normals vector
	*
	*   Return Value :
	*
	*		No return value (Void function)
	*
	*   Description :
	*
	*       Indexing enables to reuse the same vertex over and over again. This is done with an index buffer.
	*		Near indices are stored only once. This saves space and time as we don't draw same edge twice for 
	*		different triangles.
*/
void indexVBO(
	std::vector<glm::vec3>& in_vertices,
	std::vector<glm::vec2>& in_uvs,
	std::vector<glm::vec3>& in_normals,
	std::vector<unsigned short>& out_indices,
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::vec3>& out_normals
	) {
	std::map<PackedVertex, unsigned short> VertexToOutIndex;

	for (unsigned int i = 0; i < in_vertices.size(); i++) {

		PackedVertex packed = { in_vertices[i], in_uvs[i], in_normals[i] };
		// Try to find a similar vertex in out_XXXX
		unsigned short index;
		bool found = getSimilarVertexIndex(packed, VertexToOutIndex, index);

		if (found) { // A similar vertex is already in the VBO, use it instead !
			out_indices.push_back(index);
		}
		else { // If not, it needs to be added in the output data.
			out_vertices.push_back(in_vertices[i]);
			out_uvs.push_back(in_uvs[i]);
			out_normals.push_back(in_normals[i]);
			unsigned short newindex = (unsigned short)out_vertices.size() - 1;
			out_indices.push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}
	}
}