
//#include "Zone_GlowMapContainer.h"
//#include "Zone_SkinContainer.h"
#include "static_mesh.h"

namespace Render
{

StaticMesh::StaticMesh() :GeomObject()
{
//	f_pMaterial = new Material();

//	f_pRenderState = new RenderState();
//	f_pRenderState->AlphaBlend() = true;
//	f_pRenderState->AlphaBlendSrc() = AlphaBlend_SrcAlpha;
//	f_pRenderState->AlphaBlendDest() = AlphaBlend_InvSrcAlpha;
//	f_pRenderState->AlphaTest() = false;
//	f_pRenderState->CullMode() = Cull_None;
//	f_pRenderState->ZEnable() = true;
//	f_pRenderState->ZWrite() = true;
//	f_pRenderState->ZFunction() = ZFunction_LessEqual;
//	f_pRenderState->FogEnable() = true;
//	f_pRenderState->FillMode() = Fill_Solid;
//	f_pRenderState->Dither() = true;
//	f_pRenderState->ShadeMode() = Shade_Linear;

//	f_pTextureState = new TextureState();
//	f_pTextureState->SetColor(0, Texture_Modulate);
//	f_pTextureState->SetAlpha(0, Texture_Modulate);

//	f_pTexture = new Texture();
//	f_pTexture2 = new Texture();
//	f_pTexture3 = new Texture();
//	f_pTexture4 = new Texture();
}

StaticMesh::StaticMesh(const std::string& sTexName) : GeomObject()
{
//	f_pMaterial = new Material();

//	f_pRenderState = new RenderState();
//	f_pRenderState->AlphaBlend() = true;
//	f_pRenderState->AlphaBlendSrc() = AlphaBlend_SrcAlpha;
//	f_pRenderState->AlphaBlendDest() = AlphaBlend_InvSrcAlpha;
//	f_pRenderState->AlphaTest() = false;
//	f_pRenderState->CullMode() = Cull_None;
//	f_pRenderState->ZEnable() = true;
//	f_pRenderState->ZWrite() = true;
//	f_pRenderState->ZFunction() = ZFunction_LessEqual;
//	f_pRenderState->FogEnable() = true;
//	f_pRenderState->FillMode() = Fill_Solid;
//	f_pRenderState->Dither() = true;
//	f_pRenderState->ShadeMode() = Shade_Linear;

//	f_pTextureState = new TextureState();
//	f_pTextureState->SetColor(0, Texture_Modulate, Texture_Texture, Texture_Diffuse);
//	f_pTextureState->SetAlpha(0, Texture_Modulate, Texture_Texture, Texture_Diffuse);
//	f_pTextureState->SetColor(1, Texture_Disable);
//	f_pTextureState->SetAlpha(1, Texture_Disable);

//	f_pTexture = new Texture();
//	f_pTexture2 = new Texture();
//	f_pTexture3 = new Texture();
//    f_pTexture4 = new Texture();

    m_texName[0] = sTexName;
}

StaticMesh::~StaticMesh()
{
    finalize();
}


void StaticMesh::process()
{
    if (m_vertex.empty())
	{
		return;
	}

    m_min = m_max = m_vertex[0];

    for (auto& v : m_vertex)
	{
        m_min.x() = std::min(m_min.x(), v.x());
        m_max.x() = std::max(m_max.x(), v.x());

        m_min.y() = std::min(m_min.y(), v.y());
        m_max.y() = std::max(m_max.y(), v.y());

        m_min.z() = std::min(m_min.z(), v.z());
        m_max.z() = std::max(m_max.z(), v.z());
	}

    rebuild();
}

void StaticMesh::computeNormal()
{
    finalize();

    for (auto& v : m_normal)
	{
        v = Vector3::c0;
	}

	int iIndex = 0;
    for(int i = 0; i < m_index.size() / 3; ++i)
	{
        int iV0 = m_index[iIndex++];
        int iV1 = m_index[iIndex++];
        int iV2 = m_index[iIndex++];

        Vector3& zV0 = m_vertex[iV0];
        Vector3& zV1 = m_vertex[iV1];
        Vector3& zV2 = m_vertex[iV2];

        Vector3 zEdge1 = zV1 - zV0;
        Vector3 zEdge2 = zV2 - zV0;
        Vector3 zNormal = zEdge1^zEdge2;
		if((zEdge1 & zEdge2) > 0)
		{
			zEdge1 = zV0 - zV1;
			zEdge2 = zV2 - zV1;
			zNormal = zEdge1^zEdge2;
		}

        m_normal[iV0] += zNormal;
        m_normal[iV1] += zNormal;
        m_normal[iV2] += zNormal;
	}

    for(int i = 0; i < m_normal.size(); ++i)
	{
        m_normal[i].normalize();
	}
}

//void StaticMesh::ComputeOBBox()
//{
//ZONE_B("StaticMesh::ComputeOBBox");

//	f_zOBBox.Box().Center() = Vector((f_fMaxX + f_fMinX)*0.5f, SizeY()*0.5f,
//		(f_fMaxZ + f_fMinZ)*0.5f);

//	f_zOBBox.Box().Axis(0) = Vector::cX;
//	f_zOBBox.Box().Axis(1) = Vector::cY;
//	f_zOBBox.Box().Axis(2) = Vector::cZ;

//	f_zOBBox.Box().Extent(0) = SizeX()*0.5f;
//	f_zOBBox.Box().Extent(1) = SizeY()*0.5f;
//	f_zOBBox.Box().Extent(2) = SizeZ()*0.5f;

//ZONE_E;
//}

void StaticMesh::rebuild()
{
    finalize();
    computeNormal();
//	ComputeOBBox();
}

void StaticMesh::set()
{
    if (!m_initialized)
	{
        initialize();
	}

//	f_pResource->Set();
}

void StaticMesh::draw()
{
//	f_pRenderState->Apply();
	
//	int iTexCount = 1;
//	if(TexCoord2().size() > 0)
//	{
//		++iTexCount;

//		if(TexCoord3().size() > 0)
//		{
//			++iTexCount;

//			if(TexCoord4().size() > 0)
//			{
//				++iTexCount;
//			}
//		}
//	}

//	f_pTexture->Set(0);
//	if(iTexCount > 1)
//	{
//		f_pTexture2->Set(1);

//		if(iTexCount > 2)
//		{
//			f_pTexture3->Set(2);

//			if(iTexCount > 3)
//			{
//				f_pTexture4->Set(3);
//			}
//		}
//	}
//	f_pTextureState->Apply(iTexCount);
//	f_pResource->Draw(0, Vertex().size(), Index().size()/3, 0);
}

void StaticMesh::drawGlow(int iGlowIndex)
{
//	f_pRenderState->Apply();
	
//	int iTexCount = 1;
//	if(TexCoord2().size() > 0)
//	{
//		++iTexCount;

//		if(TexCoord3().size() > 0)
//		{
//			++iTexCount;

//			if(TexCoord4().size() > 0)
//			{
//				++iTexCount;
//			}
//		}
//	}

//	f_pTexture->Set(0);
//	if(iTexCount > 1)
//	{
//		f_pTexture2->Set(1);

//		if(iTexCount > 2)
//		{
//			f_pTexture3->Set(2);

//			if(iTexCount > 3)
//			{
//				f_pTexture4->Set(3);
//			}
//		}
//	}

//	f_pTextureState->Apply(iTexCount);
//	f_pResource->Draw(0, Vertex().size(), Index().size()/3, 0);
}

void StaticMesh::drawSkinned(const size_t iIndex)
{
//	f_pRenderState->Apply();
	
//	int iTexCount = 1;
//	if(TexCoord2().size() > 0)
//	{
//		++iTexCount;

//		if(TexCoord3().size() > 0)
//		{
//			++iTexCount;

//			if(TexCoord4().size() > 0)
//			{
//				++iTexCount;
//			}
//		}
//	}

//	f_pTexture->Set(0);
//	if(iTexCount > 1)
//	{
//		if(2 == iTexCount)
//		{
//			SkinContainer::GetPointer()->Set(iIndex, 1);
//		}
//		else
//		{
//			f_pTexture2->Set(1);
//		}

//		if(iTexCount > 2)
//		{
//			if(3 == iTexCount)
//			{
//				SkinContainer::GetPointer()->Set(iIndex, 2);
//			}
//			else
//			{
//				f_pTexture3->Set(2);
//			}

//			if(iTexCount > 3)
//			{
//				SkinContainer::GetPointer()->Set(iIndex, 3);
//			}
//		}
//	}
//	f_pTextureState->Apply(iTexCount);
//	f_pResource->Draw(0, Vertex().size(), Index().size()/3, 0);
}

void StaticMesh::initialize()
{
    if (m_initialized)
	{
		return;
	}

//	int iFormat = Resource::c_VertexNormal | Resource::c_VertexDiffuse;
//	if(TexCoord4().size() > 0)
//	{
//		iFormat |= Resource::c_VertexTex4;
//	}
//	else if(TexCoord3().size() > 0)
//	{
//		iFormat |= Resource::c_VertexTex3;
//	}
//	else if(TexCoord2().size() > 0)
//	{
//		iFormat |= Resource::c_VertexTex2;
//	}
//	else
//	{
//		iFormat |= Resource::c_VertexTex1;
//	}

//	f_pResource = new Resource(iFormat, true);

//	for(int i = 0; i < Vertex().size(); ++i)
//	{
//		f_pResource->AddVertex();
//		f_pResource->AddCoord(Vertex()[i].X(), Vertex()[i].Y(), Vertex()[i].Z());
//		f_pResource->AddNormal(Normal()[i].X(), Normal()[i].Y(), Normal()[i].Z());
//		f_pResource->AddColor(Color()[i]);
//		f_pResource->AddTexCoord(TexCoord()[i].f1, TexCoord()[i].f2);

//		if(TexCoord2().size() > 0)
//		{
//			f_pResource->AddTexCoord2(TexCoord2()[i].f1, TexCoord2()[i].f2);
//		}

//		if(TexCoord3().size() > 0)
//		{
//			f_pResource->AddTexCoord3(TexCoord3()[i].f1, TexCoord3()[i].f2);
//		}

//		if(TexCoord4().size() > 0)
//		{
//			f_pResource->AddTexCoord4(TexCoord4()[i].f1, TexCoord4()[i].f2);
//		}
//	}

//	for(i = 0; i < Index().size(); ++i)
//	{
//		f_pResource->AddIndex(Index()[i]);
//	}
//	f_pResource->Create();

//	ImagePtr pImage = new Image();
//	pImage->LoadFromFile("Meshes\\" + f_sTexName);
//	f_pTexture->Create(pImage);

//	if(f_sTexName2 != "")
//	{
//		pImage->LoadFromFile("Meshes\\" + f_sTexName2);
//		f_pTexture2->Create(pImage);

//		if(f_sTexName3 != "")
//		{
//			pImage->LoadFromFile("Meshes\\" + f_sTexName3);
//			f_pTexture3->Create(pImage);

//			if(f_sTexName4 != "")
//			{
//				pImage->LoadFromFile("Meshes\\" + f_sTexName4);
//				f_pTexture4->Create(pImage);
//			}
//		}
//	}

    m_initialized = true;
}

void StaticMesh::finalize()
{
    if(!m_initialized)
	{
		return;
	}

//	int iFormat = Resource::c_VertexNormal | Resource::c_VertexDiffuse;
//	if(TexCoord4().size() > 0)
//	{
//		iFormat |= Resource::c_VertexTex4;
//	}
//	else if(TexCoord3().size() > 0)
//	{
//		iFormat |= Resource::c_VertexTex3;
//	}
//	else if(TexCoord2().size() > 0)
//	{
//		iFormat |= Resource::c_VertexTex2;
//	}
//	else
//	{
//		iFormat |= Resource::c_VertexTex1;
//	}

//	f_pResource = new Resource(iFormat, true);
    m_initialized = false;
}


//void StaticMesh::LoadFromFile(const string& sFileName)
//{
//try
//{
//        pFile->Open();

//        int iSignature;
//        pFile->Read(iSignature);
//        if(0x464D535A != iSignature)
//        {
//            ZONE_THROW_E("wrong signature: " + sFileName);
//        }

//        pFile->Read(f_sName);
//        /*S
//		LocalRotate() = c_Math_Matrix3_1;
//		LocalTranslate() = c_Math_Vector0;
//		LocalScale() = 1;

//		WorldRotate().LoadFromFile(pFile);
//		WorldTranslate().LoadFromFile(pFile);
//		pFile->Read(WorldScale());
//*/
//        Matrix3 zRotate;
//        zRotate.LoadFromFile(pFile);

//        Vector zTranslate;
//        zTranslate.LoadFromFile(pFile);

//        float fScale;
//        pFile->Read(fScale);

//        f_pMaterial->LoadFromFile(pFile);
//        f_pRenderState->LoadFromFile(pFile);
//        f_pTextureState->LoadFromFile(pFile);

//        pFile->Read(f_sTexName);
//        pFile->Read(f_sTexName2);
//        pFile->Read(f_sTexName3);
//        pFile->Read(f_sTexName4);

//        int iVertex;
//        pFile->Read(iVertex);
//        Vertex().resize(iVertex);
//        Normal().resize(iVertex);
//        Color().resize(iVertex);
//        TexCoord().resize(iVertex);

//        int iFlag;
//        pFile->Read(iFlag);
//        if(iFlag&1)
//        {
//            TexCoord2().resize(iVertex);

//            if(iFlag&2)
//            {
//                TexCoord3().resize(iVertex);

//                if(iFlag&4)
//                {
//                    TexCoord4().resize(iVertex);
//                }
//            }
//        }

//        for(int i = 0; i < Vertex().size(); ++i)
//        {
//            pFile->Read(Vertex()[i].X());
//            pFile->Read(Vertex()[i].Y());
//            pFile->Read(Vertex()[i].Z());

//            Color()[i] = 0xffffffff;

//            pFile->Read(TexCoord()[i].f1);
//            pFile->Read(TexCoord()[i].f2);

//            if(iFlag & 1)
//            {
//                pFile->Read(TexCoord2()[i].f1);
//                pFile->Read(TexCoord2()[i].f2);

//                if(iFlag & 2)
//                {
//                    pFile->Read(TexCoord3()[i].f1);
//                    pFile->Read(TexCoord3()[i].f2);

//                    if(iFlag & 4)
//                    {
//                        pFile->Read(TexCoord4()[i].f1);
//                        pFile->Read(TexCoord4()[i].f2);
//                    }
//                }
//            }
//        }

//        int iIndex;
//        pFile->Read(iIndex);
//        Index().resize(iIndex);
//        for(i = 0; i < Index().size(); ++i)
//        {
//            pFile->Read(Index()[i]);
//        }

//        int iFormat = Resource::c_VertexNormal | Resource::c_VertexDiffuse;
//        if(TexCoord4().size() > 0)
//        {
//            iFormat |= Resource::c_VertexTex4;
//        }
//        else if(TexCoord3().size() > 0)
//        {
//            iFormat |= Resource::c_VertexTex3;
//        }
//        else if(TexCoord2().size() > 0)
//        {
//            iFormat |= Resource::c_VertexTex2;
//        }
//        else
//        {
//            iFormat |= Resource::c_VertexTex1;
//        }

//        f_pResource = new Resource(iFormat, true);

//        pFile->Read(f_iType);
//}
//catch(Exception)
//{
//        ZONE_THROW_E("can\'t load SMesh: " + sFileName);
//        throw;
//}

//Finalize();
//ComputeNormal();

////Snk: todo, after model will be updated
//for(int i = 0; i < 4; ++i)
//{
//        f_pTextureState->SetFilter(i, Texture_Linear, Texture_Linear);
//}

//ZONE_E;
//}

bool StaticMesh::loadFromTinygltf(const tinygltf::Mesh& mesh, const tinygltf::Model& model)
{
    if (!GeomObject::loadFromTinygltf(mesh, model))
    {
        return false;
    }

    if (m_normal.empty())
    {
        computeNormal();
    }

    return true;
}

//namespace Render
}
