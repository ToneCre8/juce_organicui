/*
  ==============================================================================

    Inspectable.h
    Created: 30 Oct 2016 9:02:24am
    Author:  bkupe

  ==============================================================================
*/

#ifndef INSPECTABLE_H_INCLUDED
#define INSPECTABLE_H_INCLUDED


class InspectableEditor;
class InspectableSelectionManager;

class Inspectable
{
public:
	Inspectable();
	virtual ~Inspectable();

	InspectableSelectionManager * selectionManager;

	bool isSelected;
	bool isSelectable;
	bool showInspectorOnSelect;

	//for multiSelection
	bool isPreselected;

	//Help
	String helpID;
	virtual String getHelpID();

	virtual void selectThis(bool addToSelection = false);
	virtual void setSelected(bool value);


	virtual void setSelectionManager(InspectableSelectionManager * selectionManager);

	void setPreselected(bool value);

	virtual void setSelectedInternal(bool value); //to be overriden

	virtual InspectableEditor * getEditor(bool /*isRoot*/) { jassert(false);  return nullptr; } //to override !

	//Listener
	class  InspectableListener
	{
	public:
		/** Destructor. */
		virtual ~InspectableListener() {}
		virtual void inspectableSelectionChanged(Inspectable *) {};
		virtual void inspectablePreselectionChanged(Inspectable *) {};
		virtual void inspectableDestroyed(Inspectable *) {};
	};

	ListenerList<InspectableListener> listeners;
	void addInspectableListener(InspectableListener* newListener) { listeners.add(newListener); }
	void removeInspectableListener(InspectableListener* listener) { listeners.remove(listener); }

	// ASYNC
	class  InspectableEvent
	{
	public:
		enum Type { SELECTION_CHANGED, PRESELECTION_CHANGED, DESTROYED};

		InspectableEvent(Type t, Inspectable * inspectable) :
			type(t), inspectable(inspectable) {}

		Type type;
		Inspectable * inspectable;
	};

	QueuedNotifier<InspectableEvent> inspectableNotifier;
	typedef QueuedNotifier<InspectableEvent>::Listener AsyncListener;

	void addAsyncInspectableListener(AsyncListener* newListener) { inspectableNotifier.addListener(newListener); }
	void addAsyncCoalescedInspectableListener(AsyncListener* newListener) { inspectableNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncInspectableListener(AsyncListener* listener) { inspectableNotifier.removeListener(listener); }

	WeakReference<Inspectable>::Master masterReference;
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Inspectable)
};



#endif  // INSPECTABLE_H_INCLUDED
