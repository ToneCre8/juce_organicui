#include "InspectableContentComponent.h"
/*
  ==============================================================================

	InspectableContentComponent.cpp
	Created: 9 May 2016 6:51:16pm
	Author:  bkupe

  ==============================================================================
*/

InspectableContentComponent::InspectableContentComponent(Inspectable * inspectable) :
	InspectableContent(inspectable),
	repaintOnSelectionChanged(true),
	rounderCornerSize(4),
	autoDrawHighlightWhenSelected(true),
	highlightColor(HIGHLIGHT_COLOR),
	autoSelectWithChildRespect(true),
	bringToFrontOnSelect(true)
{
	setWantsKeyboardFocus(true);
	setMouseClickGrabsKeyboardFocus(true);
	inspectable->addAsyncInspectableListener(this);
}

InspectableContentComponent::~InspectableContentComponent()
{
	if (!inspectable.wasObjectDeleted()) inspectable->removeAsyncInspectableListener(this);
}


void InspectableContentComponent::mouseEnter(const MouseEvent & e)
{
	//DBG("Mouse Enter : " << inspectable->getHelpID());
	if (inspectable.wasObjectDeleted()) return;
	if (HelpBox::getInstance()->overDataID.isEmpty()) HelpBox::getInstance()->setOverData(inspectable->getHelpID());
}

void InspectableContentComponent::mouseExit(const MouseEvent & e)
{
	//DBG("Mouse Exit : " << inspectable->getHelpID());
	String helpID = "";
	if (!inspectable.wasObjectDeleted()) helpID = inspectable->getHelpID();
	HelpBox::getInstance()->clearOverData(helpID);
}

void InspectableContentComponent::mouseDown(const MouseEvent & e)
{

	if (inspectable.wasObjectDeleted())
	{
		DBG("Object deleted on inspectable content, should never be there !");
		return;
	}

	if (!inspectable->isSelectable) return;
	if (!e.mods.isLeftButtonDown()) return;

	if (autoSelectWithChildRespect)
	{
		Component * c = e.eventComponent;

		bool foundAChildComponent = false;
		while (c != this)
		{
			InspectableContentComponent * ie = dynamic_cast<InspectableContentComponent *>(c);
			if (ie != nullptr)
			{
				foundAChildComponent = true;
				break;
			}
			c = c->getParentComponent();
		}

		if (!foundAChildComponent)
		{
			
			//grabKeyboardFocus();
			inspectable->selectThis(e.mods.isCommandDown() || e.mods.isShiftDown());
		}
	}
}

void InspectableContentComponent::paintOverChildren(Graphics & g)
{
	if (inspectable.wasObjectDeleted()) return;
	if (autoDrawHighlightWhenSelected && inspectable->isSelected)
	{
		g.setColour(highlightColor);
		g.drawRoundedRectangle(getMainBounds().toFloat(), rounderCornerSize, 2);
	}
}

Rectangle<int> InspectableContentComponent::getMainBounds()
{
	return getLocalBounds();

}
void InspectableContentComponent::newMessage(const Inspectable::InspectableEvent & e)
{
	if (e.type == Inspectable::InspectableEvent::SELECTION_CHANGED || e.type == Inspectable::InspectableEvent::PRESELECTION_CHANGED)
	{
		if (bringToFrontOnSelect) toFront(true);
		if (repaintOnSelectionChanged) repaint();
	}
}
	
