/////////////////////////////////////////////////////////////////////////////////
//
// This confidential and proprietary software may be used only as authorized
// by a licensing agreement from Synopsys Inc. In the event of publication,
// the following notice is applicable:
//
// (C) COPYRIGHT 2010 SYNOPSYS INC.  ALL RIGHTS RESERVED
//
// The entire notice above must be reproduced on all authorized copies.
//-----------------------------------------------------------------------------
// Filename    : perf_metric.sv
//
// Author      : Doron Meiraz, Synopsys Inc. Jul/12/10
//             
// Description : simple mechanism to measure simulation performance: how many
//               seconds per cycle (or simulation tick).
//
//
/////////////////////////////////////////////////////////////////////////////////

import "DPI" function int get_systime();

class CSimPerfMetric;

    local int m_walltime_start, m_walltime_current;
    local int m_simulation_time_start, m_simulation_time_end;
    local int m_cycle_time;

    function new(int cycle_time = -1);
        m_cycle_time = cycle_time;
    endfunction

    //
    // init_timer()
    // init counter and sample wall-time & simulation time
    //
    function void init_timer();
        m_walltime_start = get_systime();
        m_simulation_time_start = $time;
    endfunction

    //
    // stop_timer()
    // stop counter and sample wall-time & simulation time
    //
    function void stop_timer();
        m_walltime_current = get_systime();
        m_simulation_time_end = $time;
    endfunction
    
    //
    // get_timer_val()
    // returns number of seconds passed since init
    //
    function int get_timer_val();
        int curr_time = get_systime();
        return (curr_time - m_walltime_start);
    endfunction

    //
    // report()
    // returns a string with a report on current performance metrics
    //
    function string report();
        real ticks_per_second;
        real cycles_per_second;
        
        ticks_per_second = (m_simulation_time_end-m_simulation_time_start)/(m_walltime_current-m_walltime_start);
        cycles_per_second = ticks_per_second/m_cycle_time;

        report = "";
        report = $psprintf("%sCCalculateSimulationPerf Report:\n", report);
        //report = $psprintf("%s  Start time (sec) = %d\n", report, m_walltime_start) ;
        //report = $psprintf("%s  Current time (sec) = %d\n", report, m_walltime_current) ;
        report = $psprintf("%s  Simulation Time (sec) = %d\n", report, (m_simulation_time_end-m_simulation_time_start) );
        report = $psprintf("%s  Simulations Ticks Per Second = %.2f\n", report, ticks_per_second) ;
        if (m_cycle_time > -1) begin
            report = $psprintf("%s  cycles per second (Hz) = %.2f (Cycle Time = %0d)\n", report, cycles_per_second, m_cycle_time) ;
        end
    endfunction

endclass

