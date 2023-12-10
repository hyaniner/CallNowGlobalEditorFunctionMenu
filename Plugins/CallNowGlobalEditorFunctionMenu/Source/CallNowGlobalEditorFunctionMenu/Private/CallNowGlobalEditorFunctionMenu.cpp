/*
Copyright (c) 2023 hyaniner@outlook.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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
	static const FText MainMenuLabel = LOCTEXT("CallNowAtMainMenubarLabel", "CallNowGlobal");
	static const FText MainMenuToolTip = LOCTEXT("CallNowAtMainMenubarDesc", "Menu section to place functions you want to call right now.");
	
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