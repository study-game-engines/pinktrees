#include "mesh.h"

#include "exceptions.h"

namespace engine
{	
	Vertex::Vertex(Vector3 position, Vector2 texcoords, Vector3 normal)
	{
		this->position = position;
		this->texcoords = texcoords;
		this->normal = normal;
	}

	Mesh::Mesh(std::vector<Vertex> vertices)
	{
		this->vertices = vertices;
		for (int i = 0; i < vertices.size(); i++) { indices.push_back(i); }
	}

	Mesh::Mesh(aiMesh* mesh, const aiScene* scene, Material* material)
	{
		bool texcoordsAvailable = mesh->mTextureCoords[0];

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.position = Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

			if (texcoordsAvailable)
			{
				vertex.texcoords = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			}

			vertex.normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		this->material = material;
	}

	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &eboId);
		glDeleteBuffers(1, &vboId);
		glDeleteVertexArrays(1, &vaoId);
	}

	void Mesh::calculateTangents()
	{
		for (unsigned int i = 0; i < vertices.size(); i += 3)
		{
			Vector3 edge1 = vertices[i + 1].position - vertices[i].position;
			Vector3 edge2 = vertices[i + 2].position - vertices[i].position;

			Vector2 deltaUV1 = vertices[i + 1].texcoords - vertices[i].texcoords;
			Vector2 deltaUV2 = vertices[i + 2].texcoords - vertices[i].texcoords;

			// float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
			// multiplication by f is omitted to gain performance
			// it is not required since tangent is normalized later anyway
			Vector3 tangent = deltaUV2.y * edge1 - deltaUV1.y * edge2;

			// use same tangent for each of a face's three vertices
			vertices[i].tangent = tangent;
			vertices[i + 1].tangent = tangent;
			vertices[i + 2].tangent = tangent;

			// bitangent is computed in vertex shader to save memory
		}

		for (unsigned int i = 0; i < vertices.size(); i += 3)
		{
			Vector3 normal = vertices[i].normal.normalized();
			Vector3 tangent = vertices[i].tangent.normalized();

			// re-orthogonalize tangent vector using Gram-Schmidt
			vertices[i].tangent = (tangent - tangent.dot(normal) * normal).normalized();
		}
	}

	void Mesh::setup()
	{
		glGenVertexArrays(1, &vaoId);

		glBindVertexArray(vaoId);
		{
			glGenBuffers(1, &eboId);
			glGenBuffers(1, &vboId);

			glBindBuffer(GL_ARRAY_BUFFER, vboId);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(VERTICES);
			glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

			glEnableVertexAttribArray(TEXCOORDS);
			glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoords));

			glEnableVertexAttribArray(NORMALS);
			glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

			glEnableVertexAttribArray(TANGENTS);
			glVertexAttribPointer(TANGENTS, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Material* Mesh::getMaterial() { return material; }

	void Mesh::draw(ShaderProgram* program)
	{	
		if (material) {
			material->bind(program);
		}

		glBindVertexArray(vaoId);
		glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}