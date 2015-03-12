/******************************************************************************
*
* Copyright (C) 2015 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* XILINX CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*********************************************************************
 * Power nodes (power islands and power domains) related structures,
 * transition actions, and FSM definition.
 *********************************************************************/

#include "pm_power.h"
#include "pm_common.h"
#include "pm_proc.h"
#include "pm_master.h"
#include "pm_sram.h"
#include "pm_periph.h"
#include "xpfw_rom_interface.h"

/**
 * PmPwrDnHandler() - Power down island/domain
 * @nodePtr Pointer to node-structure of power island/dom to be powered off
 *
 * @return  Operation status
 *          - PM_RET_SUCCESS if transition succeeded
 *          - PM_RET_ERROR_FAILURE if pmu rom failed to set the state
 */
static u32 PmPwrDnHandler(PmNode* const nodePtr)
{
	u32 ret = PM_RET_ERROR_FAILURE;

	if (NULL == nodePtr) {
		ret = PM_RET_ERROR_FAILURE;
		goto done;
	}

	/* Call proper PMU-ROM handler as needed */
	switch (nodePtr->nodeId) {
	case NODE_FPD:
		ret = XpbrRstFpdHandler();
		if (ret == XST_SUCCESS) {
			ret = XpbrPwrDnFpdHandler();
			/*
			 * When FPD is powered off, the APU-GIC will be affected too.
			 * GIC Proxy has to take over for all wake-up sources for
			 * the APU.
			 */
			PmEnableProxyWake(&pmMasterApu_g);
		}
		break;
	case NODE_APU:
		ret = XST_SUCCESS;
		break;
	case NODE_RPU:
		ret = XpbrPwrDnRpuHandler();
		break;
	default:
		PmDbg("%s - unsupported node %s(%d)\n", __func__,
		      PmStrNode(nodePtr->nodeId), nodePtr->nodeId);
		break;
	}

	if (XST_SUCCESS != ret) {
		ret = PM_RET_ERROR_FAILURE;
		goto done;
	}

	nodePtr->currState = PM_PWR_STATE_OFF;
	ret = PM_RET_SUCCESS;

done:
	PmDbg("%s %s\n", __func__, PmStrNode(nodePtr->nodeId));
	return ret;
}

/**
 * PmPwrUpHandler() - Power up island/domain
 * @nodePtr Pointer to node-structure of power island/dom to be powered on
 *
 * @return  Operation status
 *          - PM_RET_SUCCESS if transition succeeded
 *          - PM_RET_ERROR_FAILURE if pmu rom failed to set the state
 */
static u32 PmPwrUpHandler(PmNode* const nodePtr)
{
	u32 ret = PM_RET_ERROR_FAILURE;

	PmDbg("%s %s\n", __func__, PmStrNode(nodePtr->nodeId));

	if (NULL == nodePtr) {
		goto done;
	}

	/* Call proper PMU-ROM handler as needed */
	switch (nodePtr->nodeId) {
	case NODE_FPD:
		ret = XpbrPwrUpFpdHandler();
		PmDbg("%s XpbrPwrUpFpdHandler return code #%d\n", __func__, ret);
		/* FIXME workaround for old version of pmu-rom */
		PmDbg("%s ignoring error\n", __func__);
		ret = XST_SUCCESS;
		break;
	case NODE_APU:
		ret = XST_SUCCESS;
		break;
	case NODE_RPU:
		ret = XpbrPwrUpRpuHandler();
		break;
	default:
		PmDbg("%s ERROR - unsupported node %s(%d)\n", __func__,
		      PmStrNode(nodePtr->nodeId), nodePtr->nodeId);
		break;
	}

	if (XST_SUCCESS != ret) {
		PmDbg("%s failed to power up %s PBR ERROR #%d\n", __func__,
		      PmStrNode(nodePtr->nodeId), ret);
		ret = PM_RET_ERROR_FAILURE;
		goto done;
	}

	nodePtr->currState = PM_PWR_STATE_ON;
	ret = PM_RET_SUCCESS;

done:
	return ret;
}

/* Children array definitions */
static PmNode* pmApuChildren[] = {
	&pmApuProcs_g[PM_PROC_APU_0].node,
	&pmApuProcs_g[PM_PROC_APU_1].node,
	&pmApuProcs_g[PM_PROC_APU_2].node,
	&pmApuProcs_g[PM_PROC_APU_3].node,
};

static PmNode* pmRpuChildren[] = {
	&pmRpuProcs_g[PM_PROC_RPU_0].node,
	&pmRpuProcs_g[PM_PROC_RPU_1].node,
};

static PmNode* pmFpdChildren[] = {
	&pmPowerIslandApu_g.node,
	&pmSlaveL2_g.slv.node,
	&pmSlaveSata_g.slv.node,
};

/* Operations for the Rpu power island */
static const PmNodeOps pmRpuNodeOps = {
	.sleep = PmPwrDnHandler,
	.wake = PmPwrUpHandler,
};

/* Operations for the Apu dummy power island */
static const PmNodeOps pmApuNodeOps = {
	.sleep = PmPwrDnHandler,
	.wake = PmPwrUpHandler,
};

/* Operations for the Fpd power domain */
static const PmNodeOps pmFpdNodeOps = {
	.sleep = PmPwrDnHandler,
	.wake = PmPwrUpHandler,
};

/*
 * Power Island and Power Domain definitions
 *
 * We only define those islands and domains containing more than 1 node.
 * For optimization reasons private power islands, such as APU0-island or
 * USB0-island are modeled as a feature of the node itself and are therefore
 * not described here.
 */
PmPower pmPowerIslandRpu_g = {
	.node = {
		.derived = &pmPowerIslandRpu_g,
		.nodeId = NODE_RPU,
		.typeId = PM_TYPE_PWR_ISLAND,
		.parent = NULL,
		.currState = PM_PWR_STATE_ON,
		.ops = &pmRpuNodeOps,
	},
	.children = pmRpuChildren,
	.childCnt = ARRAY_SIZE(pmRpuChildren),
};

/* Apu power island does not physically exist, therefore it has no operations */
PmPower pmPowerIslandApu_g = {
	.node = {
		.derived = &pmPowerIslandApu_g,
		.nodeId = NODE_APU,
		.typeId = PM_TYPE_PWR_ISLAND,
		.parent = &pmPowerDomainFpd_g,
		.currState = PM_PWR_STATE_ON,
		.ops = &pmApuNodeOps,
	},
	.children = pmApuChildren,
	.childCnt = ARRAY_SIZE(pmApuChildren),
};

PmPower pmPowerDomainFpd_g = {
	.node = {
		.derived = &pmPowerDomainFpd_g,
		.nodeId = NODE_FPD,
		.typeId = PM_TYPE_PWR_DOMAIN,
		.parent = NULL,
		.currState = PM_PWR_STATE_ON,
		.ops = &pmFpdNodeOps,
	},
	.children = pmFpdChildren,
	.childCnt = ARRAY_SIZE(pmFpdChildren),
};

/**
 * PmChildIsInLowestPowerState() - Checked whether the child node is in lowest
 *                                 power state
 * @nodePtr     Pointer to a node whose state should be checked
 */
static bool PmChildIsInLowestPowerState(const PmNode* const nodePtr)
{
	bool status = false;

	if ((true == IS_PROC(nodePtr->typeId)) ||
	    (true == IS_POWER(nodePtr->typeId))) {
		if (true == IS_OFF(nodePtr)) {
			status = true;
		}
	} else {
		/* Node is a slave */
		if (false == PmSlaveHasCapRequests((PmSlave*)nodePtr->derived)) {
			status = true;
		}
	}

	return status;
}

/**
 * PmHasAwakeChild() - Check whether power node has awake children
 * @power       Pointer to PmPower object to be checked
 *
 * Function checks whether any child of the power provided as argument stops
 * power from being turned off. In the case of processor or power child, that
 * can be checked by inspecting currState value. For slaves, that is not the
 * case, as slave can be in non-off state just because the off state is entered
 * when power is turned off. This is the case when power parent is common for
 * multiple nodes. Therefore, slave does not block power from turning off if
 * it is unused and not in lowest power state.
 *
 * @return      True if it has a child that is not off
 */
static bool PmHasAwakeChild(const PmPower* const power)
{
	u32 i;
	bool hasAwakeChild = false;

	for (i = 0U; i < power->childCnt; i++) {
		if (false == PmChildIsInLowestPowerState(power->children[i])) {
			hasAwakeChild = true;
			PmDbg("%s %s\n", __func__,
			      PmStrNode(power->children[i]->nodeId));
			break;
		}
	}

	return hasAwakeChild;
}

/**
 * PmOpportunisticSuspend() - After a node goes to sleep, try to power off
 *                            parents
 * @powerParent Pointer to the power node which should try to suspend, as well
 *              its parents.
 */
void PmOpportunisticSuspend(PmPower* const powerParent)
{
	PmPower* power = powerParent;

	if (NULL == powerParent) {
		goto done;
	}

	do {
		PmDbg("Opportunistic suspend attempt for %s\n",
		      PmStrNode(power->node.nodeId));

		if ((false == PmHasAwakeChild(power)) &&
			(true == HAS_SLEEP(power->node.ops))) {
			/* Call sleep function of this power node */
			power->node.ops->sleep(&power->node);
			power = power->node.parent;
		} else {
			power = NULL;
		}
	} while (NULL != power);

done:
	return;
}

/**
 * PmPowerUpTopParent() - Power up top parent in hierarchy that's currently off
 * @powerChild  Power child whose power parent has to be powered up
 *
 * @return      Status of the power up operation (always PM_RET_SUCCESS if all
 *              power parents are already powered on)
 *
 * This function turns on exactly one power parent, starting with the highest
 * level parent that's currently off. If all power parents are on, it will
 * turn on "powerChild", which was passed as an argument.
 *
 * Since MISRA-C doesn't allow recursion, there's an iterative algorithm in
 * PmTriggerPowerUp that calls this function iteratively until all power
 * nodes in the hierarchy are powered up.
 */
static u32 PmPowerUpTopParent(PmPower* const powerChild)
{
	u32 status = PM_RET_SUCCESS;
	PmPower* powerParent = powerChild;

	if (NULL == powerParent) {
		status = PM_RET_ERROR_INTERNAL;
		goto done;
	}

	/*
	 * Powering up needs to happen from the top down, so find the highest
	 * level parent that's currently still off and turn it on.
	 */
	while ((NULL != powerParent->node.parent) &&
	       (true == IS_OFF(&powerParent->node.parent->node))) {
		powerParent = powerChild->node.parent;
	}

	status = powerParent->node.ops->wake(&powerParent->node);

done:
	return status;
}

/**
 * PmTriggerPowerUp() - Triggered by child node (processor or slave) when it
 *                      needs its power islands/domains to be powered up
 * @power       Power node that needs to be powered up
 *
 * @return      Status of the power up operation.
 */
u32 PmTriggerPowerUp(PmPower* const power)
{
	u32 status = PM_RET_SUCCESS;

	if (NULL == power) {
		goto done;
	}

	/*
	 * Multiple hierarchy levels of power islands/domains may need to be
	 * turned on (always top-down).
	 * Use iterative approach for MISRA-C compliance
	 */
	while ((true == IS_OFF(&power->node)) && (PM_RET_SUCCESS == status)) {
		status = PmPowerUpTopParent(power);
	}

done:
	if (PM_RET_SUCCESS != status) {
		PmDbg("%s failed to power up: ERROR #%d\n", __func__, status);
	}

	return status;
}

/**
 * PmGetLowestParent() - Returns the first leaf parent with active children
 * @root    Pointer to a power object of which the leafs should be found
 *
 * In a parent-child tree, a leaf parent is a parent that has one or more
 * children, where none of the children are parents.
 * (Any state != 0 is considered an active state)
 *
 * Iterative algorithm for MISRA-C compliance
 *
 * @return  Pointer to an active leaf or root itself if no leafs exist
 */
static PmPower* PmGetLowestParent(PmPower* const root)
{
	PmPower* prevParent;
	PmPower* currParent = root;

	if (NULL == currParent) {
		goto done;
	}

	do {
		u32 i;
		prevParent = currParent;

		for (i = 0U; i < currParent->childCnt; i++) {
			if ((true != IS_POWER(currParent->children[i]->typeId)) ||
				(false != IS_OFF(currParent->children[i]))) {
				continue;
			}

			/* Active power child found */
			currParent = (PmPower*)currParent->children[i]->derived;
			break;
		}
	} while (currParent != prevParent);

done:
	return currParent;
}

/**
 * PmForcePowerDownChildren() - Forces power down for child nodes
 * @parent      pointer to power object whose children are to be turned off
 */
static void PmForcePowerDownChildren(const PmPower* const parent)
{
	u32 i;
	PmNode* child;
	PmProc* proc;

	for (i = 0U; i < parent->childCnt; i++) {
		child = parent->children[i];

		if ((false != PmChildIsInLowestPowerState(child)) ||
		    (true != HAS_SLEEP(child->ops))) {
			continue;
		}

		PmDbg("Powering OFF child node %s\n", PmStrNode(child->nodeId));

		/* Force the child's state to 0, which is its lowest power state */
		child->ops->sleep(child);
		child->currState = 0U;

		/* Special case: node is a processor, release slave-requirements */
		if (PM_TYPE_PROC == child->typeId) {
			proc = (PmProc*)child->derived;

			if ((NULL !=proc) && (true == proc->isPrimary)) {
				/* Notify master so it can release all requirements */
				PmMasterNotify(proc->master, PM_PROC_EVENT_FORCE_PWRDN);
			}
		}
	}

	return;
}

/**
 * PmForceDownTree() - Force power down node and all its children
 * @root        power node (island/domain) to turn off
 *
 * This is a power island or power domain, power it off from the bottom up:
 * find out if this parent has children which themselves have children.
 * Note: using iterative algorithm for MISRA-C compliance
 * (instead of recursion)
 *
 */
u32 PmForceDownTree(PmPower* const root)
{
	PmPower* lowestParent;
	u32 status = PM_RET_SUCCESS;

	if (NULL == root) {
		goto done;
	}

	do {
		lowestParent = PmGetLowestParent(root);
		PmForcePowerDownChildren(lowestParent);
		if (true == HAS_SLEEP(lowestParent->node.ops)) {
			status = lowestParent->node.ops->sleep(&lowestParent->node);
		}
	} while ((lowestParent != root) && (status == PM_RET_SUCCESS));

done:
	return status;
}
