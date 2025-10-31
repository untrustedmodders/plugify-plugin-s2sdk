/**
* =============================================================================
* s2sdk
* Copyright (C) 2023-2025 untrustedmodders
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

#include "cbaseentity.h"

class Trace {
public:
	DECLARE_VSCRIPT_CLASS2(Trace)

	using table = HSCRIPT;
	VSCRIPT_GLOBAL_FUNCTION(TraceCollideable, bool, table)
	VSCRIPT_GLOBAL_FUNCTION(TraceHull, bool, table)
	VSCRIPT_GLOBAL_FUNCTION(TraceLine, bool, table)

	// Wrapper for VScript that converts individual parameters to table, calls original method, then extracts results
	bool VScript_TraceCollideable(
		const Vector& start,
		const Vector& end,
		CEntityInstance* ent,
		const Vector* mins,
		const Vector* maxs,

		Vector& outPos,
		double& outFraction,
		bool& outHit,
		bool& outStartSolid,
		Vector& outNormal
	) {
		// Create input table
		ScriptVariant_t inputTableVar;
		g_pScriptVM->CreateTable(inputTableVar);
		table table = inputTableVar;

		// Populate input table with parameters
		g_pScriptVM->SetValue(table, "start", start);
		g_pScriptVM->SetValue(table, "end", end);
		g_pScriptVM->SetValue(table, "ent", ent->GetScriptInstance());
		if (mins) {
			g_pScriptVM->SetValue(table, "mins", *mins);
		}
		if (maxs) {
			g_pScriptVM->SetValue(table, "maxs", *maxs);
		}

		// Call the original Trace method with the table
		bool result = TraceCollideable(table);

		// Extract outputs from the table
		ScriptVariant_t posVar, fractionVar, hitVar, startsolidVar, normalVar;

		if (g_pScriptVM->GetValue(table, "pos", &posVar)) {
			outPos = posVar;
			g_pScriptVM->ReleaseValue(posVar);
		}

		if (g_pScriptVM->GetValue(table, "fraction", &fractionVar)) {
			outFraction = fractionVar;
			g_pScriptVM->ReleaseValue(fractionVar);
		}

		if (g_pScriptVM->GetValue(table, "hit", &hitVar)) {
			outHit = hitVar;
			g_pScriptVM->ReleaseValue(hitVar);
		}

		if (g_pScriptVM->GetValue(table, "startsolid", &startsolidVar)) {
			outStartSolid = startsolidVar;
			g_pScriptVM->ReleaseValue(startsolidVar);
		}

		if (g_pScriptVM->GetValue(table, "normal", &normalVar)) {
			outNormal = normalVar;
			g_pScriptVM->ReleaseValue(normalVar);
		}

		// Release input table
		g_pScriptVM->ReleaseValue(inputTableVar);

		return result;
	}

	bool VScript_TraceHull(
		const Vector& start,
		const Vector& end,
		const Vector& min,
		const Vector& max,
		int mask,
		CEntityInstance* ignore,

		Vector& outPos,
		double& outFraction,
		bool& outHit,
		CEntityInstance*& outEntHit,
		bool& outStartSolid
	) {
		// Create input table
		ScriptVariant_t inputTableVar;
		g_pScriptVM->CreateTable(inputTableVar);
		table table = inputTableVar;

		// Populate input table with parameters
		g_pScriptVM->SetValue(table, "start", start);
		g_pScriptVM->SetValue(table, "end", end);
		g_pScriptVM->SetValue(table, "min", min);
		g_pScriptVM->SetValue(table, "max", max);
		g_pScriptVM->SetValue(table, "mask", mask);
		g_pScriptVM->SetValue(table, "ignore", ignore->GetScriptInstance());

		// Call the original Trace method with the table
		bool result = TraceHull(table);

		// Extract outputs from the table
		ScriptVariant_t posVar, fractionVar, hitVar, enthitVar, startsolidVar;

		if (g_pScriptVM->GetValue(table, "pos", &posVar)) {
			outPos = posVar;
			g_pScriptVM->ReleaseValue(posVar);
		}

		if (g_pScriptVM->GetValue(table, "fraction", &fractionVar)) {
			outFraction = fractionVar;
			g_pScriptVM->ReleaseValue(fractionVar);
		}

		if (g_pScriptVM->GetValue(table, "hit", &hitVar)) {
			outHit = hitVar;
			g_pScriptVM->ReleaseValue(hitVar);
		}

		if (g_pScriptVM->GetValue(table, "enthit", &enthitVar)) {
			outEntHit = static_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(enthitVar));
			g_pScriptVM->ReleaseValue(enthitVar);
		}

		if (g_pScriptVM->GetValue(table, "startsolid", &startsolidVar)) {
			outStartSolid = startsolidVar;
			g_pScriptVM->ReleaseValue(startsolidVar);
		}

		// Release input table
		g_pScriptVM->ReleaseValue(inputTableVar);

		return result;
	}

	bool VScript_TraceLine(
		const Vector& startPos,
		const Vector& endPos,
		int mask,
		CEntityInstance* ignore,

		Vector& outPos,
		double& outFraction,
		bool& outHit,
		CEntityInstance*& outEntHit,
		bool& outStartSolid
	) {
		// Create input table
		ScriptVariant_t inputTableVar;
		g_pScriptVM->CreateTable(inputTableVar);
		table table = inputTableVar;

		// Populate input table with parameters
		g_pScriptVM->SetValue(table, "startpos", startPos);
		g_pScriptVM->SetValue(table, "endpos", endPos);
		g_pScriptVM->SetValue(table, "mask", mask);
		g_pScriptVM->SetValue(table, "ignore", ignore->GetScriptInstance());

		// Call the original Trace method with the table
		bool result = TraceLine(table);

		// Extract outputs from the table
		ScriptVariant_t posVar, fractionVar, hitVar, enthitVar, startsolidVar;

		if (g_pScriptVM->GetValue(table, "pos", &posVar)) {
			outPos = posVar;
			g_pScriptVM->ReleaseValue(posVar);
		}

		if (g_pScriptVM->GetValue(table, "fraction", &fractionVar)) {
			outFraction = fractionVar;
			g_pScriptVM->ReleaseValue(fractionVar);
		}

		if (g_pScriptVM->GetValue(table, "hit", &hitVar)) {
			outHit = hitVar;
			g_pScriptVM->ReleaseValue(hitVar);
		}

		if (g_pScriptVM->GetValue(table, "enthit", &enthitVar)) {
			outEntHit = static_cast<CEntityInstance*>(g_pScriptVM->GetInstanceValue(enthitVar));
			g_pScriptVM->ReleaseValue(enthitVar);
		}

		if (g_pScriptVM->GetValue(table, "startsolid", &startsolidVar)) {
			outStartSolid = startsolidVar;
			g_pScriptVM->ReleaseValue(startsolidVar);
		}

		// Release input table
		g_pScriptVM->ReleaseValue(inputTableVar);

		return result;
	}
};