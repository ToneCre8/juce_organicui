/*
  ==============================================================================

    InspectableSelectionManager.h
    Created: 6 Feb 2017 12:40:22pm
    Author:  Ben

  ==============================================================================
*/

#ifndef INSPECTABLESELECTIONMANAGER_H_INCLUDED
#define INSPECTABLESELECTIONMANAGER_H_INCLUDED

#include "Inspectable.h"

class InspectableSelectionManager :
	public Inspectable::InspectableListener
{
public:
	//juce_DeclareSingleton(InspectableSelectionManager, true);
	InspectableSelectionManager(bool isMainSelectionManager);
	~InspectableSelectionManager();

	static InspectableSelectionManager * mainSelectionManager;
	static InspectableSelectionManager * activeSelectionManager; //The last one having selected something, useful for key events like delete

	bool enabled;
	Array<Inspectable *> currentInspectables;
	
	template<class T>
	T * getInspectableAs();
	template<class T>
	Array<T *> getInspectablesAs();

	void setEnabled(bool value);

	void selectInspectables(Array<Inspectable *> inspectables, bool clearSelection = true, bool notify = true);
	void selectInspectable(WeakReference<Inspectable> inspectable, bool clearSelection = true, bool notify = true);
	void deselectInspectable(WeakReference<Inspectable> inspectable, bool notify = true);

	void clearSelection(bool notify = true);

	bool isEmpty();

	//From InspectableListener
	void inspectableDestroyed(Inspectable * inspectable);

	

	class  Listener
	{
	public:
		virtual ~Listener() {}
		virtual void inspectablesSelectionChanged() {};
	};

	ListenerList<Listener> listeners;
	void addSelectionListener(Listener* newListener) { listeners.add(newListener); }
	void removeSelectionListener(Listener* listener) { listeners.remove(listener); }

	class  SelectionEvent
	{
	public:
		enum Type { SELECTION_CHANGED };

		SelectionEvent(Type t, InspectableSelectionManager * ism) :
			type(t), selectionManager(ism) {}

		Type type;
		InspectableSelectionManager * selectionManager;
	};

	QueuedNotifier<SelectionEvent> selectionNotifier;
	typedef QueuedNotifier<SelectionEvent>::Listener AsyncListener;

	void addAsyncSelectionManagerListener(AsyncListener* newListener) { selectionNotifier.addListener(newListener); }
	void addAsyncCoalescedSelectionManagerListener(AsyncListener* newListener) { selectionNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncSelectionManagerListener(AsyncListener* listener) { selectionNotifier.removeListener(listener); }
};


template<class T>
T * InspectableSelectionManager::getInspectableAs()
{
	if (currentInspectables.size() == 0) return nullptr;
	return dynamic_cast<T *>(currentInspectables[0]);
}

template<class T>
Array<T*> InspectableSelectionManager::getInspectablesAs()
{
	Array<T *> result;
	for (auto &i : currentInspectables) result.add(dynamic_cast<T *>(i));
	return result;
}


#endif  // INSPECTABLESELECTIONMANAGER_H_INCLUDED
