#include <cad_module.h>
#include <cad_object.h>
#include <queue>
#include <set>

cad_route_map *Open(cad_kernel *c, cad_route_map *m);
cad_route_map *Close(cad_kernel *c, cad_route_map *m);

cad_module_begin()
	set_module_name( "����� ������� ���������. ���������" ) // ��� ����� ������������ � ������ ��������� �������
	set_module_priority( 0 ) // ������ = 0
	set_module_capability( CAP_TRACEROUTE ) // ������ ��� ������ �����������
	set_module_callbacks( Open, Close ) // ������ ��� ������������� � ���������� ������. 
cad_module_end()

// ��������� ������. �� �������� ��������� ����������, � �������� �� ���. 
struct cad_route_map_private
{
	// ������ ���� ����� ��� ������ ���������� ����������
	cad_kernel *kernel;
	// ������� ������ � ������
	std::queue<uint64_t> queue;
	// ��������� ��� �� ����������������� ����������
	std::set<cad_wire *> NewWires;
	// �������� ���� ���-�� ���...
};

// ������� ��������� ������, ������� ��� ������������ ������� �������.
// ����� ����� ��������� cad_route_map *self ������ �� ����� ������������ ���
// � �������� ����� �������������� ������ �������
uint32_t CleanUp(cad_route_map *self)
{
	delete self->sys;
	self->sys = NULL;
	self->AboutToDestroy = NULL;
	self->MakeStepInDemoMode = NULL;
	return 0;
}

// ���� ����� �������� ��������
uint32_t MakeStepInDemoMode( cad_route_map *self)
{
	// ��� ������� ����� ���������� ��� ����, ��� �� ������� 1 ��� ��������������� �����. 
	// �������� ����� ��������: ��� ��������� ������ ���. ���� ����� ������ ��� �����������,
	// ����������� � ������������ (����������������), � ������� ������ �����.
	// ������ �������� - ��� ��� �� ������ ����� �������. ����� ����� ��� �����������.
	// ���� ������� ��������� ���. ���� � ���� ���� ��������� ������� � �������� ������, �� ���� ������ � �����
	// ��� ���������, �����, � ���������� ������ ��� ����� ���, ��� ��� ������.
	
	// ��� �� ��������, ��� � ���������� ���� ���� ��������� �����, � ����� ������ ����� ����� ��������� ������,
	// � ������ ����������� ����� �������. 
	// ��� ��� ���������� � 1 ����. ����� ����������� ���� �������� � 1 ���� ���������, ���� ���������, ������� 
	// �� ��������� ����������� ���� ��������. ���� ��, �� ������� ��� LAST_ACTION_OK. ���� ���,
	// �� ���� �������� ��� ���� ����, � ��������� ����������� ����������, ������� ������� �� ����� ����.
	// ���� � �� ��� ���� �� �� ����������, �� ������� ��� ����, � ��.
	// ��� �������� ���� ����� ������������ ������� self->ReallocMap. ��������� ����� ���������� �����.
	// ����� ���� ���������������� ���������� MAP_PIN � MAP_EMPTY � ��������������� �� ��������.
	// ��� �� ���� �������� �������� ��������� self->currerntLayer, ��� �� ������ ���������� ����, ����� ���� 
	// ����� ����������.

	// ����� ���� ��� ������ ��� �������������� �����, � �������� ����� �� ����������, ����� ������� 
	// MORE_ACTIONS_IN_DEMO_MODE
	// ���� ���������� �������� �����, � ��� ���� ��� �������� ������, � ���� ��� �� ����������� �������
	// ������� MORE_ACTIONS
	// ���� ���������, ��� ������ ������ ������, ������ LAST_ACTION_OK
	return LAST_ACTION_OK;
}


// � ���� ����� �� ����������. 
// �������� ������ ������ - ������� ���� ��������� cad_route_map_private
// � ���������������� ��������� �� ������� ������ ���������.
cad_route_map *Open(cad_kernel *c, cad_route_map *m)
{
	m->sys = new cad_route_map_private;
	m->AboutToDestroy = CleanUp;
	m->MakeStepInDemoMode = MakeStepInDemoMode; 
	return m;
}

cad_route_map *Close(cad_kernel *c, cad_route_map *m)
{
	return NULL;
}