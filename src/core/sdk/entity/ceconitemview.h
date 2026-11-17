/**
 * =============================================================================
 * s2sdk
 * Copyright (C) 2023-2026 untrustedmodders
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <core/sdk/schema.hpp>
#include <econ/econ_item_interface.h>
#include <entity2/entitynetwork.h>
#include <networkvar.h>

enum class CSWeaponDefIndex : uint16_t;

class CAttributeManager;

// CEconItemAttribute and CAttributeList stay commented out in the sdk, projects keep their own copies

class CEconItemAttribute {
public:
	virtual SchemaMetaInfoHandle_t<CSchemaClassInfo> Schema_DynamicBinding() = 0;
	virtual void NetworkStateChanged(const NetworkStateChanged_t& data) = 0;
	virtual void NetworkStateChangedRemove(const NetworkStateChangedRemove_t& data) = 0;
	virtual void NetworkStateChangedLog(const char* fieldName, const char* info) = 0;
	virtual const CNetworkSerializerClassInfo* GetSerializerClassInfo() = 0;

	CNetworkVarChainer __m_pChainEntity;
	attrib_definition_index_t m_iAttributeDefinitionIndex;
	float m_flValue;
	float m_flInitialValue;
	int m_nRefundableCurrency;
	bool m_bSetBonus;
};
static_assert(sizeof(CEconItemAttribute) == 0x48);

// embedded in CEconItemView, whose NetworkVar_ wrappers override the notifications
class CAttributeList {
	DECLARE_CLASS_NOBASE(CAttributeList);

public:
	virtual SchemaMetaInfoHandle_t<CSchemaClassInfo> Schema_DynamicBinding();
	virtual void NetworkStateChanged(const NetworkStateChanged_t& data) {}
	virtual void NetworkStateChangedRemove(const NetworkStateChangedRemove_t& data) {}
	virtual void NetworkStateChangedLog(const char* fieldName, const char* info) {}
	virtual const CNetworkSerializerClassInfo* GetSerializerClassInfo();

	CNetworkUtlVarEmbedded(CEconItemAttribute, m_Attributes);
	CAttributeManager* m_pManager;
};
static_assert(sizeof(CAttributeList) == 0x78);

class CEconItemView : public IEconItemInterface {
public:
	DECLARE_SCHEMA_CLASS(CEconItemView);

	virtual void NetworkStateChanged(const NetworkStateChanged_t& data) = 0;

	SCHEMA_FIELD(CSWeaponDefIndex, m_iItemDefinitionIndex)
	SCHEMA_FIELD(int32, m_iEntityQuality)
	SCHEMA_FIELD(uint32, m_iEntityLevel)
	SCHEMA_FIELD(uint64, m_iItemID)
	SCHEMA_FIELD(uint32, m_iItemIDHigh)
	SCHEMA_FIELD(uint32, m_iItemIDLow)
	SCHEMA_FIELD(uint32, m_iAccountID)
	SCHEMA_FIELD(uint32, m_iInventoryPosition)
	SCHEMA_FIELD(bool, m_bInitialized)
	SCHEMA_FIELD_POINTER(CAttributeList, m_NetworkedDynamicAttributes)
};

class CAttributeContainer {
public:
	DECLARE_SCHEMA_CLASS_INLINE(CAttributeContainer);

	SCHEMA_FIELD(CEconItemView, m_Item)
};
