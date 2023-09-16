// Copyright Epic Games, Inc. All Rights Reserved.

#include "CallNowGlobalEditorFunctionMenu.h"

#include "ToolMenus.h"
#include "Interfaces/IMainFrameModule.h"

#define LOCTEXT_NAMESPACE "FCallNowGlobalEditorFunctionMenuModule"

void FCallNowGlobalEditorFunctionMenuModule::StartupModule()
{
	if(!IsRunningCommandlet())
	{
		IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
		MainFrameModule.OnMainFrameCreationFinished().AddRaw(this, &FCallNowGlobalEditorFunctionMenuModule::OnMainFrameLoaded);
	}
}

void FCallNowGlobalEditorFunctionMenuModule::ShutdownModule()
{
	if(!IsRunningCommandlet())
	{
		if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
		{
			IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
			MainFrameModule.OnMainFrameCreationFinished().RemoveAll(this);
		}
		UToolMenus::UnregisterOwner(this);
	}
}

void FCallNowGlobalEditorFunctionMenuModule::OnMainFrameLoaded
(TSharedPtr<SWindow> InRootWindow, bool bIsRunningStartupDialog)
{
	FToolMenuOwnerScoped OwnerScoped(this);

	static const FName MainMenuSectionName("CallNowAtMainMenuSection");
	static const FName MainMenuName("CallNowAtMainMenuName");
	static const FText MainMenuLabel = LOCTEXT("CallNowAtMainMenuLabel", "CallNow");
	static const FText MainMenuToolTip = LOCTEXT("CallNowAtMainMenuDesc", "Menu section to place functions you want to call right now.");
	
	UToolMenus* MenuTool = UToolMenus::Get();
	UToolMenu* MenuMainFrame = MenuTool->FindMenu("MainFrame.MainMenu");
	
	UToolMenu* MainMenuCallNow = MenuMainFrame->AddSubMenu("MainMenu", MainMenuSectionName, MainMenuName, MainMenuLabel,	MainMenuToolTip);

	static const FName PullDownMenuSectionGeneral("SectionGeneral");
	static const FText PullDownMenuSectionGeneralLabel = LOCTEXT("CallNowAtMainMenuGeneralSectionLabel", "General");
	
	FToolMenuSection& SectionGeneral = MainMenuCallNow->FindOrAddSection(PullDownMenuSectionGeneral);
	SectionGeneral.Label = PullDownMenuSectionGeneralLabel;

	FUIAction GeneralFunctionUIAction = FUIAction(
		FExecuteAction::CreateRaw(this, &FCallNowGlobalEditorFunctionMenuModule::GeneralTestFunction)
		, EUIActionRepeatMode::RepeatDisabled		
	);

	const FToolUIActionChoice GeneralFunctionActionChoice = FToolUIActionChoice(GeneralFunctionUIAction);

	static const FText MenuItemCallGeneralFunctionLabel = LOCTEXT("MenuItemCallGeneralFunctionLabel", "General Test Function");
	static const FText MenuItemCallGeneralFunctionTooltip = LOCTEXT("MenuItemCallGeneralFunctionDesc", "General Test Function");
	
	SectionGeneral.AddMenuEntry(
		"CallGeneralFunction"
		, MenuItemCallGeneralFunctionLabel
		, MenuItemCallGeneralFunctionTooltip
		, FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.AlertCircle")
		, GeneralFunctionActionChoice
	);

}

void FCallNowGlobalEditorFunctionMenuModule::GeneralTestFunction()
{
	UE_LOG(LogTemp, Warning, TEXT("GeneralTestFunction Called!"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCallNowGlobalEditorFunctionMenuModule, CallNowGlobalEditorFunctionMenu)