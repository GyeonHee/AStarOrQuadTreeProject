#include "Level.h"
#include "Actor/Actor.h"
//#include <iostream>

Level::Level()
{

}
Level::~Level()
{
	// null Ȯ�� �� ���� ����
	for (Actor* actor : actors)
	{
		SafeDelete(actor);
	}

	for (Actor* actor : addRequestedActors)
	{
		SafeDelete(actor);
	}

	for (Actor* actor : destroyRequestedActors)
	{
		SafeDelete(actor);
	}

	// std::vector ����
	actors.clear();
	addRequestedActors.clear();
	destroyRequestedActors.clear();
}

// ������ ���͸� �߰��� �� ���
void Level::AddActor(Actor* newActor)
{
	// ����ó��(�ߺ� ���� Ȯ��) �ʿ�

	// push_back / emplace_back : �迭 �� �ڿ� ���ο� �׸� �߰��ϴ� �Լ�

	// ��� �迭�� �߰�
	addRequestedActors.emplace_back(newActor);
}

void Level::DestroyActor(Actor* destroyedActor)
{
	// �ߺ� �˻�
	if (destroyRequestedActors.size() > 0)
	{
		for (const Actor* const actor : destroyRequestedActors)
		{
			if (actor == destroyedActor)
			{
				return;
			}
		}
	}
	// ��� �迭�� �߰�
	destroyRequestedActors.emplace_back(destroyedActor);
}

// �̺�Ʈ �Լ�

// ��ü �����ֱ�(Lifetime)�� 1���� ȣ��� (�ʱ�ȭ ����)
void Level::OnInit()
{
	for (Actor* const actor : actors)
	{
		if (actor == nullptr)
		{
			continue; // nullptr üũ
		}
		// ���� ó�� ���� Ȯ��
		if (!actor->isActive || actor->isExpired)
		{
			continue;
		}

		// �̹� ȣ��� ��ü�� �ǳʶٱ�
		if (actor->HasBegunPlay())
		{
			continue;
		}

		actor->OnInit();
	}
}

// ������ ���� ȣ�� (�ݺ��� �۾� / ���Ӽ��� �ʿ��� �۾�)
void Level::OnUpdate()
{
	for (Actor* const actor : actors)
	{
		if (actor == nullptr)
		{
			continue; // nullptr üũ
		}
		// ���� ó�� ���� Ȯ��
		if (!actor->isActive || actor->isExpired)
		{
			continue;
		}
	}
}

// �׸��� �Լ�
void Level::OnRender()
{
	// Render Pass
	for (Actor* const actor : actors)
	{
		// ���� ó�� ���� Ȯ��
		if (!actor->isActive || actor->isExpired)
		{
			continue;
		}

		// ��ο� ��
		actor->OnRender();
	}
}

void Level::ProcessAddAndDestroyActors()
{
	// actors �迭���� ���� ó��
	for (auto iterator = actors.begin(); iterator != actors.end();)
	{
		// ���� ��û�� �������� Ȯ�� �� �迭���� ����
		if ((*iterator)->isExpired)
		{
			// erase �Լ��� ����ϸ� iterator�� ��ȿȭ�Ǳ� ������
			// ��ȯ�Ǵ� ���� �����ؾ���/
			iterator = actors.erase(iterator);
			continue;
		}

		++iterator;
	}

	// destroyedActors �迭�� ��ȸ�ϸ鼭 ���� delete
	for (auto*& actor : destroyRequestedActors)
	{
		// ���ҽ� ����
		SafeDelete(actor);
	}

	// �迭 �ʱ�ȭ
	destroyRequestedActors.clear();

	// addRequestedActors �迭�� ��ȸ�ϸ鼭 ���ο� ���� add
	for (auto* const actor : addRequestedActors)
	{
		actors.emplace_back(actor);
		actor->SetOwner(this);
	}

	addRequestedActors.clear();
}



