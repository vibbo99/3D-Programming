#include "objLoader.h"
ObjLoader::ObjLoader()
{
}

ObjLoader::~ObjLoader()
{
}


void ObjLoader::LoadObj(char path[], std::vector<XMFLOAT3>& vertices, std::vector<XMFLOAT2>& uvs, std::vector<XMFLOAT3>& normals, char mtlPath[])
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<XMFLOAT3> temp_vertices;
	std::vector<XMFLOAT2> temp_uvs;
	std::vector<XMFLOAT3> temp_normals;

	FILE *file = fopen(path, "r");
	if (file == NULL) {
		std::cout << "Failed to open file" << std::endl;
	}
	else {
		while (1) {
			char lineHeader[128];
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break;
			else {
				if (strcmp(lineHeader, "v") == 0) {
					XMFLOAT3 vertex;
					fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
					temp_vertices.push_back(vertex);
				}
				else if (strcmp(lineHeader, "vt") == 0) {
					XMFLOAT2 uv;
					fscanf(file, "%f %f\n", &uv.x, &uv.y);
					temp_uvs.push_back(uv);
				}
				else if (strcmp(lineHeader, "vn") == 0) {
					XMFLOAT3 normal;
					fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
					temp_normals.push_back(normal);
				}
				else if (strcmp(lineHeader, "f") == 0) {
					std::string vertex1, vertex2, vertex3;
					unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
					int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
						&vertexIndex[0], &uvIndex[0], &normalIndex[0],
						&vertexIndex[1], &uvIndex[1], &normalIndex[1],
						&vertexIndex[2], &uvIndex[2], &normalIndex[2]);

					if (matches != 9) {
						std::cout << "File cant be read by parser" << std::endl;

					}
					vertexIndices.push_back(vertexIndex[0]);
					vertexIndices.push_back(vertexIndex[1]);
					vertexIndices.push_back(vertexIndex[2]);
					uvIndices.push_back(uvIndex[0]);
					uvIndices.push_back(uvIndex[1]);
					uvIndices.push_back(uvIndex[2]);
					normalIndices.push_back(normalIndex[0]);
					normalIndices.push_back(normalIndex[1]);
					normalIndices.push_back(normalIndex[2]);
				}
				else if (strcmp(lineHeader, "mtllib") == 0) {
					fscanf(file, "%s\n", mtlPath);
				}
			}
		}
	}

	//Indexing object

	//Vertices
	for (int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		XMFLOAT3 vertex = temp_vertices[vertexIndex - 1];
		vertices.push_back(vertex);
	}

	//Uvs
	for (int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		XMFLOAT2 vertex = temp_uvs[uvIndex - 1];
		uvs.push_back(vertex);
	}

	//Normals
	for (int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		XMFLOAT3 vertex = temp_normals[normalIndex - 1];
		normals.push_back(vertex);
	}
}
void ObjLoader::LoadTexture(LPCWSTR path, ID3D11Device* device_ptr, ID3D11Resource** texture,
	ID3D11ShaderResourceView** shader_resource_view_ptr)
{
	LPCWSTR pathName = path;
	HRESULT hr = DirectX::CreateWICTextureFromFile(
		device_ptr,
		pathName,
		texture,
		shader_resource_view_ptr,
		0
	);
}

//void ObjLoader::LoadDDSTexture(ID3D11Device* device_ptr, ID3D11ShaderResourceView* smrv)
//{
//	D3DX11_IMAGE_LOAD_INFO loadSMInfo;
//	loadSMInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
//
//	ID3D11Texture2D* SMTexture = 0;
//	hr = D3DX11CreateTextureFromFile(device_ptr, L"skymap.dds",
//		&loadSMInfo, 0, (ID3D11Resource**)&SMTexture, 0);
//
//	D3D11_TEXTURE2D_DESC SMTextureDesc;
//	SMTexture->GetDesc(&SMTextureDesc);
//
//	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
//	SMViewDesc.Format = SMTextureDesc.Format;
//	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
//	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
//	SMViewDesc.TextureCube.MostDetailedMip = 0;
//
//	hr = device_ptr->CreateShaderResourceView(SMTexture, &SMViewDesc, &smrv)
//}


void ObjLoader::LoadMtl(char path[], XMFLOAT4 &Kd, XMFLOAT4 &Ka, XMFLOAT4 &Ks, char texturePath[])
{

	FILE *file = fopen(path, "r");
	if (file == NULL) {
		std::cout << "Failed to open file" << std::endl;
	}
	else {
		while (1) {
			char lineHeader[128];
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break;
			if (strcmp(lineHeader, "Kd") == 0) {

				fscanf(file, "%f %f %f\n", &Kd.x, &Kd.y, &Kd.z);
			}
			else if (strcmp(lineHeader, "Ka") == 0) {
				fscanf(file, "%f %f %f\n", &Ka.x, &Ka.y, &Ka.z);
			}
			else if (strcmp(lineHeader, "Ks") == 0) {
				fscanf(file, "%f %f %f\n", &Ks.x, &Ks.y, &Ks.z);
			}
			else if (strcmp(lineHeader, "map_Kd") == 0) {
				fscanf(file, "%s\n", texturePath);
			}
		}
	}
}


