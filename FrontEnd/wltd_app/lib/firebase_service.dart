import 'package:firebase_database/firebase_database.dart';
import 'package:cloud_firestore/cloud_firestore.dart';

class FirebaseService {
  final DatabaseReference _databaseReference = FirebaseDatabase.instance.ref();
}

Stream<DatabaseEvent> getDepth() {
  DatabaseReference depth = FirebaseDatabase.instance.ref('/test/depth');
  return depth.onChildAdded;
}

