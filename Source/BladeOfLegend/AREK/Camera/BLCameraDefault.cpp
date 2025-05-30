// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCameraDefault.h"

UBLCameraDefault::UBLCameraDefault()
{
	ProjectionMode = ECameraProjectionMode::Orthographic;
	OrthoWidth = 1024.f;
	// Settings for good visibility
	PostProcessSettings.AutoExposureMethod = AEM_Manual;
	PostProcessSettings.AutoExposureBias = 10.2f;
	SetConstraintAspectRatio(true);
}
