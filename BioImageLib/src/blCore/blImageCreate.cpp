/// \file blImageCreate.cpp
/// \brief blImageCreate class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blImageCreate.h"

// /////////////////////////////////////////////////////////////////////////
//                            CreateImage
// /////////////////////////////////////////////////////////////////////////
Int2DImage::Pointer blImageCreate::createNewInt2DImage(int nl, int nc){

    Int2DImage::RegionType region;

    Int2DImage::IndexType start;
    start[0] = 0;
    start[1] = 0;
    region.SetIndex(start);

    Int2DImage::SizeType size;
    size[0] = nl;
    size[1] = nc;
    region.SetSize(size);

    Int2DImage::Pointer image = Int2DImage::New();
    image->SetRegions(region);
    image->Allocate();

    return image;
}

Float2DImage::Pointer blImageCreate::createNewFloat2DImage(int nl, int nc) {
    Float2DImage::RegionType region;

    Float2DImage::IndexType start;
    start[0] = 0;
    start[1] = 0;
    region.SetIndex(start);

    Float2DImage::SizeType size;
    size[0] = nl;
    size[1] = nc;
    region.SetSize(size);

    Float2DImage::Pointer image = Float2DImage::New();
    image->SetRegions(region);
    image->Allocate();

    return image;
}

Int3DImage::Pointer blImageCreate::createNewInt3DImage(int nl, int nc, int nz){

    Int3DImage::RegionType region;

    Int3DImage::IndexType start;
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;
    region.SetIndex(start);

    Int3DImage::SizeType size;
    size[0] = nl;
    size[1] = nc;
    size[2] = nz;
    region.SetSize(size);

    Int3DImage::Pointer image = Int3DImage::New();
    image->SetRegions(region);
    image->Allocate();

    return image;
}

Float3DImage::Pointer blImageCreate::createNewFloat3DImage(int nl, int nc, int nz){
    Float3DImage::RegionType region;

    Float3DImage::IndexType start;
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;
    region.SetIndex(start);

    Float3DImage::SizeType size;
    size[0] = nl;
    size[1] = nc;
    size[2] = nz;
    region.SetSize(size);

    Float3DImage::Pointer image = Float3DImage::New();
    image->SetRegions(region);
    image->Allocate();

    return image;
}


IntColor2DImage::Pointer blImageCreate::createNewIntColor2DImage(int nl, int nc){

    IntColor2DImage::RegionType region;

    IntColor2DImage::IndexType start;
    start[0] = 0;
    start[1] = 0;
    region.SetIndex(start);

    IntColor2DImage::SizeType size;
    size[0] = nl;
    size[1] = nc;
    region.SetSize(size);

    IntColor2DImage::Pointer image = IntColor2DImage::New();
    image->SetRegions(region);
    image->Allocate();

    return image;
}

FloatColor2DImage::Pointer blImageCreate::createNewFloatColor2DImage(int nl, int nc){

    FloatColor2DImage::RegionType region;

    FloatColor2DImage::IndexType start;
    start[0] = 0;
    start[1] = 0;
    region.SetIndex(start);

    FloatColor2DImage::SizeType size;
    size[0] = nl;
    size[1] = nc;
    region.SetSize(size);

    FloatColor2DImage::Pointer image = FloatColor2DImage::New();
    image->SetRegions(region);
    image->Allocate();

    return image;
}

IntColor3DImage::Pointer blImageCreate::createNewIntColor3DImage(int nl, int nc, int nz){
    IntColor3DImage::RegionType region;

    IntColor3DImage::IndexType start;
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;
    region.SetIndex(start);

    IntColor3DImage::SizeType size;
    size[0] = nl;
    size[1] = nc;
    size[2] = nz;
    region.SetSize(size);

    IntColor3DImage::Pointer image = IntColor3DImage::New();
    image->SetRegions(region);
    image->Allocate();

    return image;
}

FloatColor3DImage::Pointer blImageCreate::createNewFloatColor3DImage(int nl, int nc, int nz){

    FloatColor3DImage::RegionType region;

    FloatColor3DImage::IndexType start;
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;
    region.SetIndex(start);

    FloatColor3DImage::SizeType size;
    size[0] = nl;
    size[1] = nc;
    size[2] = nz;
    region.SetSize(size);

    FloatColor3DImage::Pointer image = FloatColor3DImage::New();
    image->SetRegions(region);
    image->Allocate();

    return image;
}
