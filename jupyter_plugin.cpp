/*
 * Copyright (c) 2020 :  G-CSC, Goethe University Frankfurt
 * Author: Arne Naegel
 * 
 * This file is part of UG4.
 * 
 * UG4 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License version 3 (as published by the
 * Free Software Foundation) with the following additional attribution
 * requirements (according to LGPL/GPL v3 §7):
 * 
 * (1) The following notice must be displayed in the Appropriate Legal Notices
 * of covered and combined works: "Based on UG4 (www.ug4.org/license)".
 * 
 * (2) The following notice must be displayed at a prominent place in the
 * terminal output of covered works: "Based on UG4 (www.ug4.org/license)".
 * 
 * (3) The following bibliography is recommended for citation and must be
 * preserved in all covered files:
 * "Reiter, S., Vogel, A., Heppner, I., Rupp, M., and Wittum, G. A massively
 *   parallel geometric multigrid solver on hierarchically distributed grids.
 *   Computing and visualization in science 16, 4 (2013), 151-164"
 * "Vogel, A., Reiter, S., Rupp, M., Nägel, A., and Wittum, G. UG4 -- a novel
 *   flexible software system for simulating pde based models on high performance
 *   computers. Computing and visualization in science 16, 4 (2013), 165-179"
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 */


#include <string>


// UG4
#include "bridge/util.h"
#include "common/ug_config.h"
#include "common/error.h"
#ifdef UG_PARALLEL
#include "pcl/pcl_util.h"
#endif

// replace this with util_domain_dependent.h or util_algebra_dependent.h
// to speed up compilation time
#include "bridge/util_domain_algebra_dependent.h"


// xwidgets
#include "xwidgets/xslider.hpp"
#include "xwidgets/xnumeral.hpp"
#include "xwidgets/xprogress.hpp"

#include "xwidgets/xtext.hpp"
#include "xwidgets/xtextarea.hpp"

#include "xwidgets/xtogglebuttons.hpp"
#include "xwidgets/xselection.hpp"
#include "xwidgets/xdropdown.hpp"
#include "xwidgets/xradiobuttons.hpp"
#include "xwidgets/xselect.hpp"
#include "xwidgets/xselectionslider.hpp"


#include "xwidgets/xbox.hpp"

#include "xproperty/xproperty.hpp"

using namespace std;
using namespace ug::bridge;


namespace ug{

namespace bridge {

struct xproperty_traits
{
	static const bool const_method = false;
	typedef TypeList<> params_type;
	typedef TypeValueList<params_type> DummyArgs;
};


//! General version.
template <typename V, typename M, typename O>
struct func_traits<xp::xproperty<V,M> O::* > : public xproperty_traits
{
	typedef V return_type;
	typedef xp::xproperty<V,M> property_type;

	template <typename TFunction>
	static return_type apply(TFunction fnc_ptr, O* obj, xproperty_traits::DummyArgs& args)
	{
		V value;
		// value = obj->value(); // for value only
		value = (obj->*fnc_ptr)();
		return value;
	}
};

/*
//! Min
template <typename T>
struct func_traits< typename T::min_property T::* > : public xproperty_traits
{
	typedef double return_type;

	template <typename TFunction>
	static return_type apply(TFunction, T* obj, xproperty_traits::DummyArgs& args)
	{ return obj->min(); };
};

//! Max
template <typename T>
struct func_traits< typename T::max_property T::* > : public xproperty_traits
{
	typedef double return_type;

	template <typename TFunction>
	static return_type apply(TFunction, T* obj, xproperty_traits::DummyArgs& args)
	{ return obj->max(); };
};

//! Step
template <typename T>
struct func_traits< typename T::step_property T::* > : public xproperty_traits
{
	typedef typename T::step_property::value_type return_type;

	template <typename TFunction>
	static return_type apply(TFunction, T* obj, xproperty_traits::DummyArgs& args)
	{ return obj->step(); };
};


//! Value
template <typename T>
struct func_traits< typename T::value_property T::* > : public xproperty_traits
{
	typedef typename T::value_property::value_type return_type;

	template <typename TFunction>
	static return_type apply(TFunction, T* obj, xproperty_traits::DummyArgs& args)
	{ return obj->value(); };
};

//! Orientation
template <typename T>
struct func_traits< typename T::orientation_property T::* > : public xproperty_traits
{
	typedef typename T::orientation_property::value_type return_type;

	template <typename TFunction>
	static return_type apply(TFunction, T* obj, xproperty_traits::DummyArgs& args)
	{ return obj->orientation(); };
};

//! Description
template <typename T>
struct func_traits< typename T::description_property T::* > : public xproperty_traits
{
	typedef typename T::description_property::value_type return_type;

	template <typename TFunction>
	static return_type apply(TFunction, T* obj, xproperty_traits::DummyArgs& args)
	{ return obj->description(); };
};

*/


}

namespace Jupyter{

/** 
 *  \defgroup sample_plugin Jupyter
 *  \ingroup plugins
 *  This is a small sample plugin.
 *  \{
 */

template <class XWidget>
struct XWidgetRegistryFacade
{

	typedef typename XWidget::value_type value_type;

	//! Create Object from json and display.
	static SmartPtr<XWidget> create(const char* jdesc)

	{
		std::string jstring(jdesc);
		nlohmann::json j = nlohmann::json::parse(jstring);

		SmartPtr<XWidget> xw = make_sp(new XWidget);
		from_json(j, *xw);

		xw->display();	//  XWidget
		return xw;
	}

	static void print(const XWidget *xw)
	{
		nlohmann::json j;
		to_json(j, *xw);
		std::cerr << j << std::endl;

	}


};




// template <typename TRegistry, >
// #define ADD_XW_GET_VALUE(name, ) (

//reg.add_function("XWidgetsSaysHello", &XWidgetRegistryFacade<>::, grp)

//)




void XWidgetsSaysHello()
{
#ifdef UG_PARALLEL
	pcl::SynchronizeProcesses();
	cout << "Hello, I'm your plugin on proc " <<
				pcl::ProcRank() << "." << endl;
	pcl::SynchronizeProcesses();
#else
	UG_LOG("Hello, I'm your personal plugin in serial environment!\n");

	// Test.
	// slider = new xw::slider<double>();
	// slider->display();

#endif
}

void CrashFct(const string& reason)
{
	UG_THROW("I Crash because: "<< reason);
}

void CrashFctFatal(const string& reason)
{
	UG_THROW("I Crash fatal because: "<< reason);
}

void XWidgetsCrashes()
{
	try{
		CrashFct("Some funny reason");
	}
	catch(bad_alloc& err)
	{
		UG_LOG("Bad Alloc");
	}
	UG_CATCH_THROW("Something wrong in XWidgetsCrashes");
}

void XWidgetsCrashesFatal()
{
	try{
		CrashFctFatal("Some fatal reason");
	}
	catch(bad_alloc& err)
	{
		UG_LOG("Bad Alloc");
	}
	UG_CATCH_THROW("Something wrong in XWidgetsCrashesFatal");
}

/**
 * Class exporting the functionality of the plugin. All functionality that is to
 * be used in scripts or visualization must be registered here.
 */
struct Functionality
{

/**
 * Function called for the registration of Domain and Algebra dependent parts
 * of the plugin. All Functions and Classes depending on both Domain and Algebra
 * are to be placed here when registering. The method is called for all
 * available Domain and Algebra types, based on the current build options.
 *
 * @param reg				registry
 * @param parentGroup		group for sorting of functionality
 */
template <typename TDomain, typename TAlgebra>
static void DomainAlgebra(Registry& reg, string grp)
{
//	useful defines
	string suffix = GetDomainAlgebraSuffix<TDomain,TAlgebra>();
	string tag = GetDomainAlgebraTag<TDomain,TAlgebra>();

}

/**
 * Function called for the registration of Domain dependent parts
 * of the plugin. All Functions and Classes depending on the Domain
 * are to be placed here when registering. The method is called for all
 * available Domain types, based on the current build options.
 *
 * @param reg				registry
 * @param parentGroup		group for sorting of functionality
 */
template <typename TDomain>
static void Domain(Registry& reg, string grp)
{
//	useful defines
	string suffix = GetDomainSuffix<TDomain>();
	string tag = GetDomainTag<TDomain>();

}

/**
 * Function called for the registration of Dimension dependent parts
 * of the plugin. All Functions and Classes depending on the Dimension
 * are to be placed here when registering. The method is called for all
 * available Dimension types, based on the current build options.
 *
 * @param reg				registry
 * @param parentGroup		group for sorting of functionality
 */
template <int dim>
static void Dimension(Registry& reg, string grp)
{
//	useful defines
	string suffix = GetDimensionSuffix<dim>();
	string tag = GetDimensionTag<dim>();

}

/**
 * Function called for the registration of Algebra dependent parts
 * of the plugin. All Functions and Classes depending on Algebra
 * are to be placed here when registering. The method is called for all
 * available Algebra types, based on the current build options.
 *
 * @param reg				registry
 * @param parentGroup		group for sorting of functionality
 */
template <typename TAlgebra>
static void Algebra(Registry& reg, string grp)
{
//	useful defines
	string suffix = GetAlgebraSuffix<TAlgebra>();
	string tag = GetAlgebraTag<TAlgebra>();

}

/**
 * Function called for the registration of Domain and Algebra independent parts
 * of the plugin. All Functions and Classes not depending on Domain and Algebra
 * are to be placed here when registering.
 *
 * @param reg				registry
 * @param parentGroup		group for sorting of functionality
 */
static void Common(Registry& reg, string grp)
{
	// First all common functions...
	reg.add_function("XWidgetsSaysHello", &XWidgetsSaysHello, grp)
		.add_function("XWidgetsCrashes", &XWidgetsCrashes, grp)
		.add_function("XWidgetsCrashesFatal", &XWidgetsCrashesFatal, grp);

	// Then go all classes...
	{
		typedef xw::slider<double> xwSlider;

		reg.add_class_<xwSlider>("xwSlider", grp, "Polynomial regions")
						.add_constructor()
						.add_method("display", &xwSlider::display, "displays object")
						.add_method("step", &xwSlider::step)
						.add_method("min", &xwSlider::min)
						.add_method("max", &xwSlider::max)
						.add_method("value", &xwSlider::value)
						.add_method("description", &xwSlider::description)
						.add_method("orientation", &xwSlider::orientation)
						.construct_as_smart_pointer();
	}

	{
		typedef xw::numeral<double> xwNumeral;

		reg.add_class_<xwNumeral>("xwNumeral", grp)
		   .add_constructor()
		   .add_method("display", &xwNumeral::display, "displays object")
		   .add_method("value", &xwNumeral::value)
		   .construct_as_smart_pointer();
	}

	{
		typedef xw::text xwText;
		reg.add_class_<xwText>("xwText", grp)
		   .add_constructor()
		   .add_method("display", &xwText::display, "displays object")
		   .add_method("value", &xwText::value)
		   .construct_as_smart_pointer();
	}

	{
		typedef xw::textarea xwTextArea;
		reg.add_class_<xwTextArea>("xwTextArea", grp)
		   .add_constructor()
		   .add_method("display", &xwTextArea::display, "displays object")
		   .add_method("value", &xwTextArea::value)
		   .construct_as_smart_pointer();
	}



	{
		typedef typename xw::togglebuttons xwToggleButtons;
		reg.add_class_<xwToggleButtons>("xwToggleButtons", grp)
		   .template add_constructor<void (*)(std::vector<std::string>,std::string)>("Function(s)#Subset(s)")
		   .add_method("display", &xwToggleButtons::display, "displays object")
		   .add_method("value", &xwToggleButtons::value)
		   .construct_as_smart_pointer();
	}

	{
		typedef typename xw::dropdown xwDropDown;
		reg.add_class_<xwDropDown>("xwDropDown", grp)
		   .template add_constructor<void (*)(std::vector<std::string>,std::string)>("Function(s)#Subset(s)")
		   .add_method("display", &xwDropDown::display, "displays object")
		   .add_method("value", &xwDropDown::value)
		   .construct_as_smart_pointer();
	}

	{
		typedef typename xw::radiobuttons xwRadioButtons;
		reg.add_class_<xwRadioButtons>("xwRadioButtons", grp)
		   .template add_constructor<void (*)(std::vector<std::string>,std::string)>("Function(s)#Subset(s)")
		   .add_method("display", &xwRadioButtons::display, "displays object")
		   .add_method("value", &xwRadioButtons::value)
		   .construct_as_smart_pointer();
	}

	{
		typedef typename xw::select xwSelect;
		reg.add_class_<xwSelect>("xwSelect", grp)
		   .template add_constructor<void (*)(std::vector<std::string>,std::string)>("Function(s)#Subset(s)")
		   .add_method("display", &xwSelect::display, "displays object")
		   .add_method("value", &xwSelect::value)
		   .construct_as_smart_pointer();
	}

	{
		typedef typename xw::selectionslider xwSelectionSlider;
		reg.add_class_<xwSelectionSlider>("xwSelectionSlider", grp)
		   .template add_constructor<void (*)(std::vector<std::string>,std::string)>("Function(s)#Subset(s)")
		   .add_method("display", &xwSelectionSlider::display, "displays object")
		   .add_method("value", &xwSelectionSlider::value)
		   .construct_as_smart_pointer();
	}

	{
			typedef typename xw::progress<double> xwProgress;
			reg.add_class_<xwProgress>("xwProgress", grp)
			   .template add_constructor<void (*)()>("Function(s)#Subset(s)")
			   .add_method("display", &xwProgress::display, "displays object")
			   .add_method("value", &xwProgress::value)
			   .construct_as_smart_pointer();
	}




	// Container
	{
		typedef typename xw::vbox xwVBox;
		reg.add_class_<xwVBox>("xwVBox", grp)
		   .template add_constructor<void (*)()>()
		   .add_method("display", &xwVBox::display, "displays object")
		   //.add_method("add", &(xwVBox::*add)())
		   //.add_method("remove", &(xwVBox::*remove)())
		   .add_method("clear", &xwVBox::clear)
		   .construct_as_smart_pointer();
	}
}


}; // end Functionality

// end group sample_plugin
/// \}

}// end of namespace Jupyter


/**
 * This function is called when the plugin is loaded.
 */
extern "C" void
InitUGPlugin_Jupyter(Registry* reg, string grp)
{
	grp.append("/Jupyter");
	typedef Jupyter::Functionality Functionality;

	try{
		RegisterCommon<Functionality>(*reg,grp);
		RegisterDimensionDependent<Functionality>(*reg,grp);
		RegisterDomainDependent<Functionality>(*reg,grp);
		RegisterAlgebraDependent<Functionality>(*reg,grp);
		RegisterDomainAlgebraDependent<Functionality>(*reg,grp);
	}
	UG_REGISTRY_CATCH_THROW(grp);
}

extern "C" UG_API void
FinalizeUGPlugin_Jupyter()
{
}

}//	end of namespace ug
