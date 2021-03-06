#include "generated_flux.h"

registerMooseObject("lizardApp", generated_flux);

template <>
InputParameters
validParams<generated_flux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addClassDescription("Electric potential generated due to heat flux");
  params.addRequiredCoupledVar("temperature", "temperature");
  params.addRequiredCoupledVar("potential_E_int", "electric potential");
  params.addParam<MaterialPropertyName>(
      "thermal_conductivity",
      "Thermal Conductivity",
      "Property name of the thermal conductivity material property");
  params.addParam<MaterialPropertyName>(
      "electrical_conductivity",
      "Electrical Conductivity",
      "Property name of the electrical conductivity material property");
  params.addParam<MaterialPropertyName>(
      "seebeck_coefficient",
      "Seebeck coefficient",
      "Property name of the Seebeck coefficient material property");
  params.addRequiredParam<unsigned int>("component",
                                        "An integer corresponding to the direction the variable "
                                        "this kernel acts in. (0 for x, 1 for y, 2 for z)");
  return params;
}

generated_flux::generated_flux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _temperature(coupledValue("temperature")),
    _temperature_grad(coupledGradient("temperature")),
    _potential_E_int(coupledValue("potential_E_int")),
    _potential_E_int_grad(coupledGradient("potential_E_int")),
    _thermal_conductivity(getMaterialProperty<Real>("thermal_conductivity")),
    _electrical_conductivity(getMaterialProperty<Real>("electrical_conductivity")),
    _seebeck_coefficient(getMaterialProperty<Real>("seebeck_coefficient")),
    _component(getParam<unsigned int>("component"))
{
}

Real
generated_flux::computeValue()
{
  return -_thermal_conductivity[_qp] * _temperature_grad[_qp](_component) -
         _seebeck_coefficient[_qp] * _temperature[_qp] *
             (_electrical_conductivity[_qp] * _potential_E_int_grad[_qp](_component) +
              _seebeck_coefficient[_qp] * _electrical_conductivity[_qp] *
                  _temperature_grad[_qp](_component));
}
