#include "Inspectable.h"
/*
  ==============================================================================

	Inspectable.cpp
	Created: 30 Oct 2016 9:02:24am
	Author:  bkupe

  ==============================================================================
*/


Inspectable::Inspectable() :
	selectionManager(nullptr), //default nullptr will target main selectionManager
	isSelected(false),
	isSelectable(true),
	showInspectorOnSelect(true),
	inspectableNotifier(10)
{
	setSelectionManager(nullptr);
}

Inspectable::~Inspectable()
{
	listeners.call(&InspectableListener::inspectableDestroyed, this);
	inspectableNotifier.addMessage(new InspectableEvent(InspectableEvent::DESTROYED, this));
	masterReference.clear();
}


String Inspectable::getHelpID()
{
	return helpID;
}

void Inspectable::selectThis(bool addToSelection)
{
	if (selectionManager == nullptr) return;
	if (!isSelectable) return;

	selectionManager->selectInspectable(this, !addToSelection);
	//if (helpID.isNotEmpty() && HelpBox::getInstanceWithoutCreating() != nullptr) HelpBox::getInstance()->setPersistentData(helpID);
}

void Inspectable::setSelected(bool value)
{
	if (!isSelectable) return;
	if (value == isSelected) return;

	isSelected = value;
	isPreselected = false;

	setSelectedInternal(value);

	listeners.call(&InspectableListener::inspectableSelectionChanged, this);
	inspectableNotifier.addMessage(new InspectableEvent(InspectableEvent::SELECTION_CHANGED, this));
}

void Inspectable::setSelectionManager(InspectableSelectionManager * _selectionManager)
{
	if (selectionManager == _selectionManager && selectionManager != nullptr) return;

	if (_selectionManager != nullptr)
	{
		selectionManager = _selectionManager;
	} else if (InspectableSelectionManager::mainSelectionManager != nullptr)
	{
		selectionManager = InspectableSelectionManager::mainSelectionManager;
	}

}

void Inspectable::setPreselected(bool value)
{
	if (!isSelectable) return;
	if (value == isPreselected) return;

	isPreselected = value;

	listeners.call(&InspectableListener::inspectablePreselectionChanged, this);
	inspectableNotifier.addMessage(new InspectableEvent(InspectableEvent::PRESELECTION_CHANGED, this));
}

void Inspectable::setSelectedInternal(bool)
{
	//to be overriden
}
