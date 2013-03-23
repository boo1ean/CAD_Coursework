﻿#include <cad_module.h>
#include <cad_object.h>
#include <queue>
#include <set>

cad_route_map *Open(cad_kernel *c, cad_route_map *m);
cad_route_map *Close(cad_kernel *c, cad_route_map *m);

cad_module_begin()
	set_module_name( "Метод путевых координат. Янушкевич" ) // это будет отображаться в списке доступных модулей
	set_module_priority( 0 ) // всегда = 0
	set_module_capability( CAP_TRACEROUTE ) // значит это модуль трассировки
	set_module_callbacks( Open, Close ) // методы для инициализации и отключения модуля. 
cad_module_end()

// привытные данные. не заводите глобальне переменные, а хранивте всё тут. 
struct cad_route_map_private
{
	// объект ядра нужен для вывода отладочной информации
	cad_kernel *kernel;
	// очередь обхода в ширину
	std::deque<uint64_t> queue;
	// множество еще не протрассированных соединений
	std::set<cad_wire *> NewWires;
	// возможно надо что-то еще...
};

// удалить привытные данные, закрыть все используемые модулем ресурсы.
// после этого структура cad_route_map *self больше не будет принадлежать вам
// и вероятно будет использоваться другим модулем
uint32_t AboutToDestroy(cad_route_map *self)
{
	delete self->sys;
	self->sys = NULL;
	self->AboutToDestroy = NULL;
	self->MakeStepInDemoMode = NULL;
	return 0;
}

// очистить все следы прибывания
uint32_t Clear(cad_route_map *self)
{
	for (uint32_t i = 0; i < self->height * self->width * self->depth; i++)
	{
		if (self->map[i] & MAP_PIN != MAP_PIN) 
			self->map[i] = MAP_EMPTY;
	}
	
	// переинициализировать внутренние структуры
	self->sys->queue.clear();
	self->sys->NewWires.clear();

	self->sys->NewWires.insert(self->sheme->connections.wire, 
		self->sheme->connections.wire + self->sheme->connections.number_of_wires);
	return 0;
}

// этот метод реализут алгоритм
uint32_t MakeStepInDemoMode( cad_route_map *self)
{
	// эта функция будет вызываться для того, что бы сделать 1 шаг распространения волны. 
	// возможны такие ситуации: она вызвалась первый раз. надо найти провод для трассировки,
	// определится с приоритетеми (алгоримозависимо), и сделать первую волну.
	// другая ситуация - это уже не первый вызов функции. часть волны уже проставлена.
	// надо сделать следующий шаг. если в этом шаге произошла встреча с конечной точеой, то надо убрать с карты
	// все стрелочки, цифры, и проставить вместо них линию там, где она прошла.
	
	// так же возможно, что в предидущем шаге была проложена линия, в таком случае нужно найти следующий провод,
	// и начать трассировку этого провода. 
	// все это происходит в 1 слое. когда трассировка всех проводов в 1 слое закончена, надо проверить, удалось 
	// ли выполнить траммировку всех проводов. если да, то вернуть код LAST_ACTION_OK. если нет,
	// то надо добавить еще один слой, и выполнить трассировку соединений, которые отслись на новом слое.
	// если и на том слое не всё получилось, то создать еще один, и тд.
	// для создания слоя нужно использовать функцию self->ReallocMap. принимает новое количество слоев.
	// новые слои инициализируются значениями MAP_PIN и MAP_EMPTY в соответствующих им позициях.
	// так же надо изменить значение перемнной self->currerntLayer, что бы модуль прорисовки знал, какой слой 
	// нужно отображать.

	// после того как сделан шаг распростанения волны, и конечная точка не достигнута, нужно вернуть 
	// MORE_ACTIONS_IN_DEMO_MODE
	// если достигнута конечная точка, и при этом был проложен провод, и есть еще не проложенные провода
	// вернуть MORE_ACTIONS
	// если оказалось, что делать больше нечего, вернуь LAST_ACTION_OK
	return LAST_ACTION_OK;
}


// у всех такая же реализация. 
// основная задача метода - создать вашу структуру cad_route_map_private
// и инициализировать указатели на функции вашими функциями.
cad_route_map *Open(cad_kernel *c, cad_route_map *m)
{
	m->sys = new cad_route_map_private;
	m->AboutToDestroy = AboutToDestroy;
	m->MakeStepInDemoMode = MakeStepInDemoMode; 
	m->Clear = Clear;
	m->Clear( m );
	return m;
}

cad_route_map *Close(cad_kernel *c, cad_route_map *m)
{
	return NULL;
}