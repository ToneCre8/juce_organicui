/*
 ==============================================================================

 Parameter.h
 Created: 8 Mar 2016 1:08:19pm
 Author:  bkupe

 ==============================================================================
 */

#ifndef PARAMETER_H_INCLUDED
#define PARAMETER_H_INCLUDED

 //Listener
class Parameter;
class TargetParameter;
class PlayableParameterAutomation;

class ParameterListener
{
public:
	/** Destructor. */
	virtual ~ParameterListener() {}
	virtual void parameterValueChanged(Parameter *) {};
	virtual void parameterRangeChanged(Parameter *) {};
	virtual void parameterControlModeChanged(Parameter *) {}
};

class Parameter : 
	public Controllable,
	public ExpressionListener,
	public ParameterListener
{
public:
	enum ControlMode {
		MANUAL,
		EXPRESSION,
		REFERENCE,
		AUTOMATION
	};

    Parameter(const Type &type, const String & niceName, const String &description, var initialValue, var minValue, var maxValue, bool enabled = true);
	virtual ~Parameter();

    var defaultValue;
    var value;
    var lastValue;


	//Range
	bool canHaveRange;
    var minimumValue;
    var maximumValue;

	var defaultMinValue;//for autoAdaptRange
	var defaultMaxValue;
   
	//Control Mode
	bool lockManualControlMode;
	ControlMode controlMode;

	//Expression
	String controlExpression;
	ScopedPointer<ScriptExpression> expression;
	WeakReference<Controllable> controlReference;

	//Reference
	ScopedPointer<TargetParameter> referenceTarget;
	Parameter * referenceParameter;

	//Automation
	ScopedPointer<PlayableParameterAutomation> automation;

	bool isComplex();
	virtual StringArray getValuesNames();

	virtual void setRange(var,var, bool setDefaultRange = true);
	virtual void clearRange();
	virtual bool hasRange();

	bool isPresettable;
    bool isOverriden;
	bool autoAdaptRange;
	bool forceSaveValue; //if true, will save value even if not overriden


	void setControlMode(ControlMode _mode);
	void setControlExpression(const String &);
	void setReferenceParameter(Parameter * tp);

	virtual var getValue(); //may be useful, or testing expression or references (for now, forward update from expression timer)
	virtual var getLerpValueTo(var targetValue, float weight);
	virtual void setWeightedValue(Array<var> values, Array<float> weights) {} // to be overriden

    void resetValue(bool silentSet = false);
	virtual UndoableAction * setUndoableValue(var oldValue, var newValue, bool onlyReturnAction = false);
    virtual void setValue(var _value, bool silentSet = false, bool force = false, bool forceOverride = true);
    virtual void setValueInternal(var & _value);

	virtual bool checkValueIsTheSame(var newValue, var oldValue); //can be overriden to modify check behavior

    //For Number type parameters
	void setUndoableNormalizedValue(const float &oldNormalizedValue, const float &newNormalizedValue);
	void setNormalizedValue(const float &normalizedValue, bool silentSet = false, bool force = false);
    float getNormalizedValue();

    //helpers for fast typing
    virtual float floatValue() { return (float)getValue(); }
	virtual double doubleValue(){return (double)getValue();}
	virtual int intValue() { return (int)getValue(); }
	virtual bool boolValue() { return (bool)getValue(); }
	virtual String stringValue() { return getValue().toString(); }

    void notifyValueChanged();

	//From Script Expression
	virtual void expressionValueChanged(ScriptExpression *) override;
	virtual void expressionStateChanged(ScriptExpression *) override;
	

	//Reference
	virtual void parameterValueChanged(Parameter * p) override;


	InspectableEditor * getEditor(bool isRoot) override;

	virtual var getJSONDataInternal() override;
	virtual void loadJSONDataInternal(var data) override;
	
	static var getValueFromScript(const juce::var::NativeFunctionArgs &a);

    ListenerList<ParameterListener> listeners;
    void addParameterListener(ParameterListener* newListener) { listeners.add(newListener); }
    void removeParameterListener(ParameterListener* listener) { listeners.remove(listener); }

    // ASYNC
    class  ParameterEvent
	{
    public:
		enum Type { VALUE_CHANGED, BOUNDS_CHANGED, CONTROLMODE_CHANGED, EXPRESSION_STATE_CHANGED };

		ParameterEvent(Type t,Parameter * p,var v = var()) :
			type(t),parameter(p),value(v)
		{
		}

		Type type;
        Parameter * parameter;
        var value;
    };

    QueuedNotifier<ParameterEvent> queuedNotifier;
    typedef QueuedNotifier<ParameterEvent>::Listener AsyncListener;


    void addAsyncParameterListener(AsyncListener* newListener) { queuedNotifier.addListener(newListener); }
    void addAsyncCoalescedParameterListener(AsyncListener* newListener) { queuedNotifier.addAsyncCoalescedListener(newListener); }
    void removeAsyncParameterListener(AsyncListener* listener) { queuedNotifier.removeListener(listener); }
	 


	bool checkVarIsConsistentWithType();
	WeakReference<Parameter>::Master masterReference;
	friend class WeakReference<Parameter>;


public:
	class ParameterAction :
		public ControllableAction
	{
	public:
		ParameterAction(Parameter * param) :
			ControllableAction(param),
			parameterRef(param)
		{
			controlAddress = param->getControlAddress();
		}

		WeakReference<Parameter> parameterRef;
		String controlAddress;

		Parameter * getParameter();
	};

	class ParameterSetValueAction :
		public ParameterAction
	{
	public:
		ParameterSetValueAction(Parameter * param, var oldValue, var newValue) :
			ParameterAction(param),
			oldValue(oldValue), 
			newValue(newValue)
		{
			//DBG("New Parameter Set Value Action");
		}

		var oldValue;
		var newValue;
		
		bool perform() override;
		bool undo() override;
	};



	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Parameter)

};


#endif  // PARAMETER_H_INCLUDED
