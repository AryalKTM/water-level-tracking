import 'package:firebase_core/firebase_core.dart';
import "package:flutter/material.dart";
import 'package:wltd_app/splash.dart';
import 'package:wltd_app/firebase_options.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(
    options: DefaultFirebaseOptions.currentPlatform,
  );

  runApp(const waterApp());
}

class waterApp extends StatelessWidget {
  const waterApp({super.key});

  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      debugShowCheckedModeBanner: false,
      title: "Water Level Tracking System",
      home: SplashScreen(),
    );
  }
}