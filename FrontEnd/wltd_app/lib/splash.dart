import 'dart:async';
import 'package:flutter/material.dart';
import 'package:wltd_app/home.dart';
import 'package:google_fonts/google_fonts.dart';

class SplashScreen extends StatefulWidget {
  const SplashScreen({super.key});
  @override
  State<SplashScreen> createState() => _SplashScreenState();
}

class _SplashScreenState extends State<SplashScreen> {
  @override
  void initState() {
    Timer(const Duration(seconds: 2), () {
      Navigator.push(
          context, MaterialPageRoute(builder: (context) => const HomePage()));
    });
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: const Color(0xff151e3f),
      body: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          Center(
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                const Icon(
                  Icons.water_drop_outlined,
                  size: 100,
                  color: Color(0xfff2f3d9),
                ),
                const SizedBox(
                  height: 20,
                ),
                Text(
                  "Water Level Tracking Dashboard",
                  style: GoogleFonts.kanit(
                    textStyle: const TextStyle(
                      fontSize: 22,
                      color: Color(0xffdb6332),
                    ),
                  ),
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }
}
