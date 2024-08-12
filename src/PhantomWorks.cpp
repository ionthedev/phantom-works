//
// Created by Brandon Friend on 8/10/24.
//
//
// Created by Brandon Friend on 8/7/24.
//

#include <iostream>

#include "PhantomPch.h"
#include "Core/EntryPoint.h"

class PhantomWorks : public Phantom::Application
{
public:
    PhantomWorks()
    {

    }

    ~PhantomWorks()
    {
    }

    void Run() override;
};

void PhantomWorks::Run()
{
    Application::Run();

}

Phantom::Application* Phantom::CreateApplication()
{
    return new PhantomWorks();
}

