// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "WolfAdventure/Characters/WolfCharacter.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWolfCharacter() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_ACharacter();
	ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputAction_NoRegister();
	ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputMappingContext_NoRegister();
	UPackage* Z_Construct_UPackage__Script_WolfAdventure();
	WOLFADVENTURE_API UClass* Z_Construct_UClass_AWolfCharacter();
	WOLFADVENTURE_API UClass* Z_Construct_UClass_AWolfCharacter_NoRegister();
// End Cross Module References
	void AWolfCharacter::StaticRegisterNativesAWolfCharacter()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AWolfCharacter);
	UClass* Z_Construct_UClass_AWolfCharacter_NoRegister()
	{
		return AWolfCharacter::StaticClass();
	}
	struct Z_Construct_UClass_AWolfCharacter_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WolfMappingContext_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_WolfMappingContext;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MoveAction_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_MoveAction;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_LookAction_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_LookAction;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_JumpAction_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_JumpAction;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_EquipAction_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_EquipAction;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AttackAction_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_AttackAction;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_DodgeAction_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_DodgeAction;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_timeBetweenJumps_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_timeBetweenJumps;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_timeBetweenAttacks_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_timeBetweenAttacks;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_isAttacking_MetaData[];
#endif
		static void NewProp_isAttacking_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_isAttacking;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AWolfCharacter_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_ACharacter,
		(UObject* (*)())Z_Construct_UPackage__Script_WolfAdventure,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AWolfCharacter_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWolfCharacter_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "Characters/WolfCharacter.h" },
		{ "ModuleRelativePath", "Characters/WolfCharacter.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWolfCharacter_Statics::NewProp_WolfMappingContext_MetaData[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Characters/WolfCharacter.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AWolfCharacter_Statics::NewProp_WolfMappingContext = { "WolfMappingContext", nullptr, (EPropertyFlags)0x0020080000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AWolfCharacter, WolfMappingContext), Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AWolfCharacter_Statics::NewProp_WolfMappingContext_MetaData), Z_Construct_UClass_AWolfCharacter_Statics::NewProp_WolfMappingContext_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWolfCharacter_Statics::NewProp_MoveAction_MetaData[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Characters/WolfCharacter.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AWolfCharacter_Statics::NewProp_MoveAction = { "MoveAction", nullptr, (EPropertyFlags)0x0020080000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AWolfCharacter, MoveAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AWolfCharacter_Statics::NewProp_MoveAction_MetaData), Z_Construct_UClass_AWolfCharacter_Statics::NewProp_MoveAction_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWolfCharacter_Statics::NewProp_LookAction_MetaData[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Characters/WolfCharacter.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AWolfCharacter_Statics::NewProp_LookAction = { "LookAction", nullptr, (EPropertyFlags)0x0020080000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AWolfCharacter, LookAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AWolfCharacter_Statics::NewProp_LookAction_MetaData), Z_Construct_UClass_AWolfCharacter_Statics::NewProp_LookAction_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWolfCharacter_Statics::NewProp_JumpAction_MetaData[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Characters/WolfCharacter.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AWolfCharacter_Statics::NewProp_JumpAction = { "JumpAction", nullptr, (EPropertyFlags)0x0020080000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AWolfCharacter, JumpAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AWolfCharacter_Statics::NewProp_JumpAction_MetaData), Z_Construct_UClass_AWolfCharacter_Statics::NewProp_JumpAction_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWolfCharacter_Statics::NewProp_EquipAction_MetaData[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Characters/WolfCharacter.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AWolfCharacter_Statics::NewProp_EquipAction = { "EquipAction", nullptr, (EPropertyFlags)0x0020080000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AWolfCharacter, EquipAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AWolfCharacter_Statics::NewProp_EquipAction_MetaData), Z_Construct_UClass_AWolfCharacter_Statics::NewProp_EquipAction_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWolfCharacter_Statics::NewProp_AttackAction_MetaData[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Characters/WolfCharacter.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AWolfCharacter_Statics::NewProp_AttackAction = { "AttackAction", nullptr, (EPropertyFlags)0x0020080000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AWolfCharacter, AttackAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AWolfCharacter_Statics::NewProp_AttackAction_MetaData), Z_Construct_UClass_AWolfCharacter_Statics::NewProp_AttackAction_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWolfCharacter_Statics::NewProp_DodgeAction_MetaData[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Characters/WolfCharacter.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AWolfCharacter_Statics::NewProp_DodgeAction = { "DodgeAction", nullptr, (EPropertyFlags)0x0020080000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AWolfCharacter, DodgeAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AWolfCharacter_Statics::NewProp_DodgeAction_MetaData), Z_Construct_UClass_AWolfCharacter_Statics::NewProp_DodgeAction_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWolfCharacter_Statics::NewProp_timeBetweenJumps_MetaData[] = {
		{ "Category", "WolfCharacter" },
		{ "ModuleRelativePath", "Characters/WolfCharacter.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AWolfCharacter_Statics::NewProp_timeBetweenJumps = { "timeBetweenJumps", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AWolfCharacter, timeBetweenJumps), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AWolfCharacter_Statics::NewProp_timeBetweenJumps_MetaData), Z_Construct_UClass_AWolfCharacter_Statics::NewProp_timeBetweenJumps_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWolfCharacter_Statics::NewProp_timeBetweenAttacks_MetaData[] = {
		{ "Category", "WolfCharacter" },
		{ "ModuleRelativePath", "Characters/WolfCharacter.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AWolfCharacter_Statics::NewProp_timeBetweenAttacks = { "timeBetweenAttacks", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AWolfCharacter, timeBetweenAttacks), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AWolfCharacter_Statics::NewProp_timeBetweenAttacks_MetaData), Z_Construct_UClass_AWolfCharacter_Statics::NewProp_timeBetweenAttacks_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWolfCharacter_Statics::NewProp_isAttacking_MetaData[] = {
		{ "Category", "WolfCharacter" },
		{ "ModuleRelativePath", "Characters/WolfCharacter.h" },
	};
#endif
	void Z_Construct_UClass_AWolfCharacter_Statics::NewProp_isAttacking_SetBit(void* Obj)
	{
		((AWolfCharacter*)Obj)->isAttacking = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_AWolfCharacter_Statics::NewProp_isAttacking = { "isAttacking", nullptr, (EPropertyFlags)0x0020080000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AWolfCharacter), &Z_Construct_UClass_AWolfCharacter_Statics::NewProp_isAttacking_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AWolfCharacter_Statics::NewProp_isAttacking_MetaData), Z_Construct_UClass_AWolfCharacter_Statics::NewProp_isAttacking_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AWolfCharacter_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AWolfCharacter_Statics::NewProp_WolfMappingContext,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AWolfCharacter_Statics::NewProp_MoveAction,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AWolfCharacter_Statics::NewProp_LookAction,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AWolfCharacter_Statics::NewProp_JumpAction,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AWolfCharacter_Statics::NewProp_EquipAction,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AWolfCharacter_Statics::NewProp_AttackAction,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AWolfCharacter_Statics::NewProp_DodgeAction,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AWolfCharacter_Statics::NewProp_timeBetweenJumps,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AWolfCharacter_Statics::NewProp_timeBetweenAttacks,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AWolfCharacter_Statics::NewProp_isAttacking,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_AWolfCharacter_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AWolfCharacter>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AWolfCharacter_Statics::ClassParams = {
		&AWolfCharacter::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_AWolfCharacter_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_AWolfCharacter_Statics::PropPointers),
		0,
		0x009000A4u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AWolfCharacter_Statics::Class_MetaDataParams), Z_Construct_UClass_AWolfCharacter_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AWolfCharacter_Statics::PropPointers) < 2048);
	UClass* Z_Construct_UClass_AWolfCharacter()
	{
		if (!Z_Registration_Info_UClass_AWolfCharacter.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AWolfCharacter.OuterSingleton, Z_Construct_UClass_AWolfCharacter_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AWolfCharacter.OuterSingleton;
	}
	template<> WOLFADVENTURE_API UClass* StaticClass<AWolfCharacter>()
	{
		return AWolfCharacter::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AWolfCharacter);
	AWolfCharacter::~AWolfCharacter() {}
	struct Z_CompiledInDeferFile_FID_WolfAdventure_Source_WolfAdventure_Characters_WolfCharacter_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_WolfAdventure_Source_WolfAdventure_Characters_WolfCharacter_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AWolfCharacter, AWolfCharacter::StaticClass, TEXT("AWolfCharacter"), &Z_Registration_Info_UClass_AWolfCharacter, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AWolfCharacter), 2116786367U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_WolfAdventure_Source_WolfAdventure_Characters_WolfCharacter_h_4064291616(TEXT("/Script/WolfAdventure"),
		Z_CompiledInDeferFile_FID_WolfAdventure_Source_WolfAdventure_Characters_WolfCharacter_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_WolfAdventure_Source_WolfAdventure_Characters_WolfCharacter_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
