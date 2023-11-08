// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "WolfAdventure/Characters/WolfAnimInstance.h"
#include "../../Source/Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWolfAnimInstance() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_UAnimInstance();
	UPackage* Z_Construct_UPackage__Script_WolfAdventure();
	WOLFADVENTURE_API UClass* Z_Construct_UClass_UWolfAnimInstance();
	WOLFADVENTURE_API UClass* Z_Construct_UClass_UWolfAnimInstance_NoRegister();
// End Cross Module References
	void UWolfAnimInstance::StaticRegisterNativesUWolfAnimInstance()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UWolfAnimInstance);
	UClass* Z_Construct_UClass_UWolfAnimInstance_NoRegister()
	{
		return UWolfAnimInstance::StaticClass();
	}
	struct Z_Construct_UClass_UWolfAnimInstance_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UWolfAnimInstance_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UAnimInstance,
		(UObject* (*)())Z_Construct_UPackage__Script_WolfAdventure,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UWolfAnimInstance_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWolfAnimInstance_Statics::Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "AnimInstance" },
		{ "IncludePath", "Characters/WolfAnimInstance.h" },
		{ "ModuleRelativePath", "Characters/WolfAnimInstance.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UWolfAnimInstance_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWolfAnimInstance>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UWolfAnimInstance_Statics::ClassParams = {
		&UWolfAnimInstance::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009000A8u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UWolfAnimInstance_Statics::Class_MetaDataParams), Z_Construct_UClass_UWolfAnimInstance_Statics::Class_MetaDataParams)
	};
	UClass* Z_Construct_UClass_UWolfAnimInstance()
	{
		if (!Z_Registration_Info_UClass_UWolfAnimInstance.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UWolfAnimInstance.OuterSingleton, Z_Construct_UClass_UWolfAnimInstance_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UWolfAnimInstance.OuterSingleton;
	}
	template<> WOLFADVENTURE_API UClass* StaticClass<UWolfAnimInstance>()
	{
		return UWolfAnimInstance::StaticClass();
	}
	UWolfAnimInstance::UWolfAnimInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UWolfAnimInstance);
	UWolfAnimInstance::~UWolfAnimInstance() {}
	struct Z_CompiledInDeferFile_FID_WolfAdventure_Source_WolfAdventure_Characters_WolfAnimInstance_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_WolfAdventure_Source_WolfAdventure_Characters_WolfAnimInstance_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UWolfAnimInstance, UWolfAnimInstance::StaticClass, TEXT("UWolfAnimInstance"), &Z_Registration_Info_UClass_UWolfAnimInstance, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UWolfAnimInstance), 2664848011U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_WolfAdventure_Source_WolfAdventure_Characters_WolfAnimInstance_h_3042698105(TEXT("/Script/WolfAdventure"),
		Z_CompiledInDeferFile_FID_WolfAdventure_Source_WolfAdventure_Characters_WolfAnimInstance_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_WolfAdventure_Source_WolfAdventure_Characters_WolfAnimInstance_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
