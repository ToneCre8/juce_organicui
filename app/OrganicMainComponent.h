#pragma once


class OrganicMainContentComponent   : 
	public Component, 
	public ApplicationCommandTarget, 
	public MenuBarModel, 
	public EngineListener
{
public:
   	OrganicMainContentComponent();
    virtual ~OrganicMainContentComponent();


	TooltipWindow tooltipWindow; //just declare one here

#if JUCE_OPENGL
	OpenGLContext openGLContext;
#endif

	ScopedPointer<LookAndFeelOO> lookAndFeelOO;
	ScopedPointer<ProgressWindow> fileProgressWindow;


	virtual void init();

	virtual void clear();

    virtual void paint (Graphics&) override;
    virtual void resized() override;


	//engine
	virtual void startLoadFile() override;
	virtual void fileProgress(float percent, int state) override;
	virtual void endLoadFile() override;

	// inherited from MenuBarModel , ApplicationCommandTarget
	ApplicationCommandTarget* getNextCommandTarget() override { return findFirstTargetParentComponent(); }
	virtual void getAllCommands(Array<CommandID>& commands) override;
	virtual void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
	virtual bool perform(const InvocationInfo& info) override;
	virtual StringArray getMenuBarNames() override;
	virtual PopupMenu getMenuForIndex(int topLevelMenuIndex, const String& menuName) override;
	virtual void menuItemSelected(int /*menuItemID*/, int /*topLevelMenuIndex*/) override;




private:
    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OrganicMainContentComponent)

};
