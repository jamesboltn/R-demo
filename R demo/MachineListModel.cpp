#pragma once
#include"MachineListModel.h"
#include"SQLite.h"

void MachineListModel::CreateTable(SQLite* db, DatabaseErrorMessageHandler handler)
{
	db->AddFieldNotNull("location", TEXT);
	db->AddFieldNotNull("lan_ip", TEXT);
	db->AddFieldNotNull("public_domain", TEXT);
	db->AddFieldNotNull("roles", TEXT);
	db->AddFieldNotNull("modules", TEXT);
	(*handler)(db->CreateTable("machine_list"));
}