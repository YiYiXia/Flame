/*----------- Copyright(c) 2018 YimingXia(491691865@qq.com)-------------------
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files(the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions :
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#pragma once
#include"solver/Solver.h"
#include"solver/SolverMPM.h"
#include"solver/SolverMPMMulti.h"
#include"solver/SolverMAC.h"
#include"../scene/Scene.h"
#include"../simulation/ParticleSample.h"

/********************************************************
	*Class name:       Simulator
	*Usage:            handle the information between solver and scene
	*Last Update       2018.08.10 Yiming Xia
 ***********************************************************/
class Simulator
{
public:
	Simulator();
	~Simulator();
	Solver* sol;//solver
	Scene* sc;//the scene used to solve
/********************************************************
	*Function name:    SimulatorInitial.
	*Usage:            Initial the information in scene:Sample particles
	                   and build the solver.
	*Input:            scene,solver information:solver type,integral type.
	*Output:           void.
	*Last Update       2018.08.10 Yiming Xia
 ***********************************************************/
	void SimulatorInitial(Scene* scene, SolverType solverType, IntegralType integralType);

/********************************************************
	*Function name:    SimulatorUpdate.
	*Usage:            Update scene and solver with the work process.
	*Input:            none.
	*Output:           none.
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
	void SimulatorUpdate();
private:

/********************************************************
	*Function name:    InformationCheck
	*Usage:            Check whether the jet should work,
	                   whether the particle boundary should work.
	*Input:            none
	*Output:           none
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
	void InformationCheck();

/********************************************************
	*Function name:    InformationUpdate
	*Usage:            Check whether the jet should work.
	*Input:            none
	*Output:           none
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
	void InformationUpdate();
};