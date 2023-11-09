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
	ENGINE_API UClass* Z_Construct_UClass_UCharacterMovementComponent_NoRegister();
	UPackage* Z_Construct_UPackage__Script_WolfAdventure();
	WOLFADVENTURE_API UClass* Z_Construct_UClass_AWolfCharacter_NoRegister();
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
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WolfCharacter_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_WolfCharacter;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WolfCharacterMovement_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_WolfCharacterMovement;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_GroundSpeed_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_GroundSpeed;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_IsFalling_MetaData[];
#endif
		static void NewProp_IsFalling_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_IsFalling;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
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
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_WolfCharacter_MetaData[] = {
		{ "Category", "WolfAnimInstance" },
		{ "ModuleRelativePath", "Characters/WolfAnimInstance.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_WolfCharacter = { "WolfCharacter", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWolfAnimInstance, WolfCharacter), Z_Construct_UClass_AWolfCharacter_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_WolfCharacter_MetaData), Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_WolfCharacter_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_WolfCharacterMovement_MetaData[] = {
		{ "Category", "Movement" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Characters/WolfAnimInstance.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_WolfCharacterMovement = { "WolfCharacterMovement", nullptr, (EPropertyFlags)0x001000000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWolfAnimInstance, WolfCharacterMovement), Z_Construct_UClass_UCharacterMovementComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_WolfCharacterMovement_MetaData), Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_WolfCharacterMovement_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_GroundSpeed_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Characters/WolfAnimInstance.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_GroundSpeed = { "GroundSpeed", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWolfAnimInstance, GroundSpeed), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_GroundSpeed_MetaData), Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_GroundSpeed_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_IsFalling_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Characters/WolfAnimInstance.h" },
	};
#endif
	void Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_IsFalling_SetBit(void* Obj)
	{
		((UWolfAnimInstance*)Obj)->IsFalling = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_IsFalling = { "IsFalling", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UWolfAnimInstance), &Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_IsFalling_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_IsFalling_MetaData), Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_IsFalling_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UWolfAnimInstance_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_WolfCharacter,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_WolfCharacterMovement,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_GroundSpeed,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWolfAnimInstance_Statics::NewProp_IsFalling,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UWolfAnimInstance_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWolfAnimInstance>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UWolfAnimInstance_Statics::ClassParams = {
		&UWolfAnimInstance::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UWolfAnimInstance_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UWolfAnimInstance_Statics::PropPointers),
		0,
		0x009000A8u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UWolfAnimInstance_Statics::Class_MetaDataParams), Z_Construct_UClass_UWolfAnimInstance_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UWolfAnimInstance_Statics::PropPointers) < 2048);
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
		{ Z_Construct_UClass_UWolfAnimInstance, UWolfAnimInstance::StaticClass, TEXT("UWolfAnimInstance"), &Z_Registration_Info_UClass_UWolfAnimInstance, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UWolfAnimInstance), 2557160979U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_WolfAdventure_Source_WolfAdventure_Characters_WolfAnimInstance_h_637350289(TEXT("/Script/WolfAdventure"),
		Z_CompiledInDeferFile_FID_WolfAdventure_Source_WolfAdventure_Characters_WolfAnimInstance_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_WolfAdventure_Source_WolfAdventure_Characters_WolfAnimInstance_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
