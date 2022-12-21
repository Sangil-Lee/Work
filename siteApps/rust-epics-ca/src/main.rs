// Simple helper for library

use futures::executor::block_on;
use epics_ca::*;
use humantime::format_rfc3339;
use std::{thread, time};

fn main()
{
    block_on(async {
        let pv = "SR-DI-DCCT-01:SIGNAL";

        let sec = time::Duration::from_millis(1000);

        loop {
            let result: f64 = CA::caget(pv).await;
            println!("Caget: {} => {}", pv, result);

            thread::sleep(sec);
        }
    });
}
