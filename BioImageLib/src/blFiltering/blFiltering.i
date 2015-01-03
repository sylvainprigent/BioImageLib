%module blFiltering

%include <std_string.i>

%{

#include "../blCore/blProcessObserver.h"
#include "../blCore/blProcess.h"
#include "blFilter.h"
#include "blBilateralFilter.h"
#include "blMedianFilter.h"

%}
%include "../blCore/blProcessObserver.h"
%include "../blCore/blProcess.h"
%include "blFilter.h"
%include "blBilateralFilter.h"
%include "blMedianFilter.h"
