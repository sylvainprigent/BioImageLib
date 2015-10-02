/// \file blKernelConvolution.h
/// \brief blKernelConvolution class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <blCore>
#include "blFilteringExport.h"

/// \class blKernelConvolution
/// \brief Create a kernel for the blConvolution filter
/// from a blMatrix or blMatrix3
class BLFILTERING_EXPORT blKernelConvolution{

public:
    /// \fn blKernelConvolution(const blMatrix &matrix);
    /// \brief Construct a kernel from a blMatrix
    /// \param[in] matrix Matrix to use for the kernel
    blKernelConvolution(const blMatrix &matrix);
    /// \fn blKernelConvolution(const blMatrix3D &matrix);
    /// \brief Construct a kernel from a blMatrix
    /// \param[in] matrix Matrix to use for the kernel
    blKernelConvolution(const blMatrix3D &matrix);

    /// \fn blImage* kernel();
    /// \return the generated kernel
    blImage* kernel();

protected:
    blImage* m_kernel; ///< pointer to the kernel
};
