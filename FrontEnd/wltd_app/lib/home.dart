import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:percent_indicator/circular_percent_indicator.dart';
import 'package:wltd_app/firebase_service.dart';

class HomePage extends StatefulWidget {
  const HomePage({super.key});

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  bool light = true;

  double calculatePercent(depth) {
    var percent = (depth / 115) * 100;
    return percent;
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: const Color(0xfff2f3d9),
      appBar: AppBar(
        leading: const Icon(
          Icons.water_drop_sharp,
          color: Color(0xfff2f3d9),
        ),
        title: Text(
          "Water Level Tracking Dashboard",
          style: GoogleFonts.kanit(
            textStyle: const TextStyle(
              color: Color(0xffdb6332),
              fontWeight: FontWeight.bold,
              fontSize: 20,
            ),
          ),
        ),
        backgroundColor: const Color(0xff151e3f),
      ),
      body: SafeArea(
        child: Column(
          children: [
            Container(
              decoration: BoxDecoration(
                  color: const Color(0xff151e3f),
                  borderRadius: BorderRadius.circular(25),
                  boxShadow: [
                    BoxShadow(
                      color: Colors.black.withOpacity(0.15),
                      spreadRadius: 1,
                      blurRadius: 6,
                      offset: const Offset(3, 7),
                    ),
                  ]),
              padding: const EdgeInsets.all(15),
              margin: const EdgeInsets.fromLTRB(10, 20, 10, 10),
              height: 200,
              width: double.infinity,
              child: Row(
                children: [
                  Column(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Text(
                        "Water",
                        style: GoogleFonts.kanit(
                          textStyle: const TextStyle(
                            fontSize: 40,
                            color: Color(0xfff2f3d9),
                            fontWeight: FontWeight.bold,
                          ),
                        ),
                      ),
                      Text(
                        "Level",
                        style: GoogleFonts.kanit(
                          textStyle: const TextStyle(
                            fontSize: 40,
                            color: Color(0xfff2f3d9),
                            fontWeight: FontWeight.bold,
                          ),
                        ),
                      ),
                    ],
                  ),
                  const SizedBox(
                    width: 100,
                  ),
                  StreamBuilder<Object>(
                      stream: getDepth(),
                      builder: (context, snapshot) {
                        double percent = 0;
                        if (snapshot.data != null) {
                          DatabaseEvent depth = snapshot.data as DatabaseEvent;
                          percent = calculatePercent(depth.snapshot.value);
                        } 
                        return CircularPercentIndicator(
                            radius: 75,
                            lineWidth: 10,
                            percent: percent > 0 && percent <= 100
                                ? percent / 100
                                : 0,
                            animation: true,
                            circularStrokeCap: CircularStrokeCap.round,
                            progressColor: const Color(0xffdb6332),
                            center: Text(
                              "${percent.toStringAsFixed(2)}%",
                              style: GoogleFonts.kanit(
                                textStyle: const TextStyle(
                                  fontSize: 30,
                                  color: Color(0xfff2f3d9),
                                  fontWeight: FontWeight.w400,
                                ),
                              ),
                            ));
                      }),
                ],
              ),
            ),
            Container(
              padding: const EdgeInsets.all(15),
              margin: const EdgeInsets.fromLTRB(10, 20, 10, 10),
              height: 100,
              decoration: BoxDecoration(
                  color: const Color(0xff151e3f),
                  borderRadius: BorderRadius.circular(15),
                  boxShadow: [
                    BoxShadow(
                      color: Colors.black.withOpacity(0.2),
                      spreadRadius: 1,
                      blurRadius: 6,
                      offset: const Offset(3, 7),
                    ),
                  ]),
              child: Row(
                children: [
                  Text(
                    "Motor Status",
                    style: GoogleFonts.kanit(
                      textStyle: const TextStyle(
                        fontSize: 30,
                        color: Color(0xfff2f3d9),
                        fontWeight: FontWeight.bold,
                      ),
                    ),
                  ),
                  const SizedBox(
                    width: 120,
                  ),
                  Switch(
                    value: light,
                    onChanged: ((value) {
                      setState(() {
                        light = value;
                      });
                    }),
                    activeColor: const Color(0xffdb6332),
                  ),
                ],
              ),
            ),
          ],
        ),
      ),
      bottomNavigationBar: Container(
        height: 70,
        color: const Color(0xff151e3f),
        child: Row(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            const Icon(
              Icons.copyright_rounded,
              color: Color(0xffdb6332),
            ),
            const SizedBox(
              width: 5,
            ),
            Text(
              "Bipashree Aryal",
              style: GoogleFonts.kanit(
                textStyle: const TextStyle(
                  fontSize: 28,
                  color: Color(0xfff2f3d9),
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
