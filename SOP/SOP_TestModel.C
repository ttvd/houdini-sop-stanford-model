#include "SOP_TestModel.h"
#include "SOP_TestModelBunny.h"
#include "SOP_TestModelDragon.h"
#include "SOP_TestModelBuddha.h"
#include "SOP_TestModelKitten.h"

#include <UT/UT_DSOVersion.h>
#include <UT/UT_Interrupt.h>
#include <OP/OP_Operator.h>
#include <OP/OP_OperatorTable.h>
#include <PRM/PRM_Include.h>
#include <GEO/GEO_Detail.h>
#include <GEO/GEO_PrimPoly.h>
#include <GU/GU_Detail.h>


static PRM_Name s_name_create_normals("sfm_create_normals", "Create normals");
static PRM_Name s_name_keep_original_coordinate_system("sfm_keep_original_coordinate_system", "Keep Original Coordinate System");
static PRM_Name s_name_model_type("sfm_model_type", "Model Type");
static PRM_Name s_name_model_types[] =
{
    PRM_Name("bunny", "Bunny"),
    PRM_Name("dragon", "Dragon"),
    PRM_Name("buddha", "Buddha"),
    PRM_Name("kitten", "Kitten"),
    PRM_Name(0),
};
static PRM_Name s_name_primitive_type("sfm_primitive_type", "Primitive Type");
static PRM_Name s_name_primitive_types[] =
{
    PRM_Name("poly", "Polygon"),
    PRM_Name("points", "Points"),
    PRM_Name(0),
};


static PRM_Default s_default_create_normals(true);
static PRM_Default s_default_keep_original_coordinate_system(false);
static PRM_Range s_default_uscale_range(PRM_RANGE_UI, 0, PRM_RANGE_UI, 10);


static PRM_ChoiceList s_choicelist_model_type(PRM_CHOICELIST_SINGLE, s_name_model_types);
static PRM_ChoiceList s_choicelist_primitive_type(PRM_CHOICELIST_SINGLE, s_name_primitive_types);


PRM_Template
SOP_TestModel::myTemplateList[] = {
    PRM_Template(PRM_ORD, 1, &s_name_model_type, 0, &s_choicelist_model_type),
    PRM_Template(PRM_ORD, 1, &s_name_primitive_type, 0, &s_choicelist_primitive_type),
    PRM_Template(PRM_TOGGLE, 1, &s_name_keep_original_coordinate_system, &s_default_keep_original_coordinate_system),
    PRM_Template(PRM_XYZ, 3, &PRMcenterName),
    PRM_Template(PRM_FLT, 1, &PRMuscaleName, PRMoneDefaults, 0, &s_default_uscale_range),
    PRM_Template(PRM_TOGGLE, 1, &s_name_create_normals, &s_default_create_normals),
    PRM_Template()
};


int
SOP_TestModel::getModelType(fpreal t) const
{
    return evalInt("sfm_model_type", 0, t);
}


int
SOP_TestModel::getPrimitiveType(fpreal t) const
{
    return evalInt("sfm_primitive_type", 0, t);
}


bool
SOP_TestModel::getCreateNormals(fpreal t) const
{
    return evalInt("sfm_create_normals", 0, t);
}


bool
SOP_TestModel::getKeepOriginalCoordinateSystem(fpreal t) const
{
    return evalInt("sfm_keep_original_coordinate_system", 0, t);
}


float
SOP_TestModel::getUniformScale(fpreal t) const
{
    return evalFloat("scale", 0, t);
}


float
SOP_TestModel::getCenterX(fpreal t) const
{
    return evalFloat("t", 0, t);
}


float
SOP_TestModel::getCenterY(fpreal t) const
{
    return evalFloat("t", 1, t);
}


float
SOP_TestModel::getCenterZ(fpreal t) const
{
    return evalFloat("t", 2, t);
}


OP_Node*
SOP_TestModel::myConstructor(OP_Network* network, const char* name, OP_Operator* op)
{
    return new SOP_TestModel(network, name, op);
}


SOP_TestModel::SOP_TestModel(OP_Network* network, const char* name, OP_Operator* op) :
    SOP_Node(network, name, op)
{

}


SOP_TestModel::~SOP_TestModel()
{

}


OP_ERROR
SOP_TestModel::cookMySop(OP_Context& context)
{
    if(error() >= UT_ERROR_ABORT)
    {
        return error();
    }

    gdp->clearAndDestroy();

    UT_AutoInterrupt boss("Building Test Model");
    if(boss.wasInterrupted())
    {
        return error();
    }

    fpreal t = context.getTime();

    int mode_type = getModelType(t);
    int primitive_type = getPrimitiveType(t);
    bool keep_original_coordinate_system = getKeepOriginalCoordinateSystem(t);
    bool create_normals = getCreateNormals(t);
    float uniform_scale = getUniformScale(t);
    UT_Vector3 model_center(getCenterX(t), getCenterY(t), getCenterZ(t));

    int num_vertices = 0;
    int num_indices = 0;
    int num_normals = 0;

    const float* data_vertices = nullptr;
    const float* data_indices = nullptr;
    const float* data_normals = nullptr;

    switch(mode_type)
    {
        default:
        case 0:
        {
            num_vertices = SOP_TestModelBunny::s_num_vertices;
            num_indices = SOP_TestModelBunny::s_num_indices;

            data_vertices = &SOP_TestModelBunny::s_vertices[0];
            data_indices = &SOP_TestModelBunny::s_indices[0];

            break;
        }

        case 1:
        {
            num_vertices = SOP_TestModelDragon::s_num_vertices;
            num_indices = SOP_TestModelDragon::s_num_indices;

            data_vertices = &SOP_TestModelDragon::s_vertices[0];
            data_indices = &SOP_TestModelDragon::s_indices[0];

            break;
        }

        case 2:
        {
            num_vertices = SOP_TestModelBuddha::s_num_vertices;
            num_indices = SOP_TestModelBuddha::s_num_indices;

            data_vertices = &SOP_TestModelBuddha::s_vertices[0];
            data_indices = &SOP_TestModelBuddha::s_indices[0];

            break;
        }

        case 3:
        {
            num_vertices = SOP_TestModelKitten::s_num_vertices;
            num_indices = SOP_TestModelKitten::s_num_indices;
            num_normals = SOP_TestModelKitten::s_num_normals;

            data_vertices = &SOP_TestModelKitten::s_vertices[0];
            data_indices = &SOP_TestModelKitten::s_indices[0];
            data_normals = &SOP_TestModelKitten::s_normals[0];

            break;
        }
    }

    GA_RWHandleV3 attr_normal;

    if(create_normals && num_normals > 0)
    {
        attr_normal = GA_RWHandleV3(gdp->findFloatTuple(GA_ATTRIB_POINT, "N", 3));
        if(!attr_normal.isValid())
        {
            attr_normal.bind(gdp->addFloatTuple(GA_ATTRIB_POINT, "N", 3));
        }
    }

    UT_Map<UT_Vector3, GA_Offset> unique_points;

    for(int idx = 0; idx < num_indices; idx += 3)
    {
        GEO_PrimPoly* prim_poly = nullptr;

        if(!primitive_type)
        {
            prim_poly = dynamic_cast<GEO_PrimPoly*>(gdp->appendPrimitive(GEO_PRIMPOLY));
            prim_poly->setSize(0);
        }

        for(int vtx_idx = 0; vtx_idx < 3; ++vtx_idx)
        {
            int idx_value = *(data_indices + idx + vtx_idx);
            const float* vtx = data_vertices + 3 * idx_value;

            UT_Vector3 point_data(*(vtx + 0), *(vtx + 1), *(vtx + 2));
            point_data -= model_center;
            point_data *= uniform_scale;

            if(keep_original_coordinate_system)
            {
                point_data = UT_Vector3(point_data.x(), point_data.z(), point_data.y());
            }

            GA_Offset point_offset = 0;

            if(unique_points.contains(point_data))
            {
                point_offset = unique_points[point_data];
            }
            else
            {
                point_offset = gdp->appendPointOffset();
                gdp->setPos3(point_offset, point_data);
                unique_points[point_data] = point_offset;

                if(attr_normal.isValid())
                {
                    const float* nrm = data_normals + 3 * idx_value;
                    UT_Vector3 point_normal(*(nrm + 0), *(nrm + 1), *(nrm + 2));
                    attr_normal.set(point_offset, point_normal);
                }
            }

            if(prim_poly)
            {
                prim_poly->appendVertex(point_offset);
            }
        }

        if(prim_poly)
        {
            prim_poly->close();
        }
    }

    return error();
}


void
newSopOperator(OP_OperatorTable* table)
{
    table->addOperator(new OP_Operator("testmodel", "Test Model", SOP_TestModel::myConstructor,
        SOP_TestModel::myTemplateList, 0, 0, 0, OP_FLAG_GENERATOR));
}
