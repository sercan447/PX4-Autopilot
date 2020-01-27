/****************************************************************************
 *
 *   Copyright (c) 2020 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#include "HealthAndArmingChecks.hpp"

HealthAndArmingChecks::HealthAndArmingChecks(ModuleParams *parent, const vehicle_status_flags_s &status_flags,
		vehicle_status_s &status)
	: ModuleParams(parent),
	  _context(status_flags, status),
	  _status(status)
{
}

void HealthAndArmingChecks::update(bool force_reporting)
{
	_reporter.reset(_context.status().nav_state);

	for (unsigned i = 0; i < sizeof(_checks) / sizeof(_checks[0]); ++i) {
		if (!_checks[i]) {
			break;
		}

		_checks[i]->checkAndReport(_context, _reporter);
	}

	_reporter.finalize();
	_reporter.report(force_reporting);
}

void HealthAndArmingChecks::updateParams()
{
	for (unsigned i = 0; i < sizeof(_checks) / sizeof(_checks[0]); ++i) {
		if (!_checks[i]) {
			break;
		}

		_checks[i]->updateParams();
	}
}