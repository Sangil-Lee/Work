
`include "perf_metric.sv"

program main;


class CVeryComplicatedStuff;
    rand int a[500], b[500];
    constraint con {
        // some dummy constraint which takes long time to resolve
        foreach ( a[i] ) {
            (i < 50) -> a[i%9] > b[i%11] + a[(i*7)] ;
        }
    }
endclass : CVeryComplicatedStuff








initial begin

    `define CYCLE_TIME 100
    CSimPerfMetric sim_perf = new( `CYCLE_TIME );
    
    string s;

    // start
    sim_perf.init_timer();
     
    //
    // do simulation stuff ...
    //
    begin
        CVeryComplicatedStuff sim = new();
        for (int i = 0; i < 999; i++) begin 
            sim.randomize();
            #120;

            if (0==i%100) begin
                $display( $psprintf("simulation time is: %7d ticks, simulation is running for %5d seconds (%6d ticks/second)", 
                            $time, sim_perf.get_timer_val(), $time/sim_perf.get_timer_val() ) );
            end
        end
    end


    // end
    sim_perf.stop_timer();
    s = sim_perf.report();
    $display("%s", s);

end



endprogram 

