pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
                sh 'echo Hello World > testArtifact.rtf'
				archiveArtifacts artifacts: 'testArtifact.rtf', fingerprint: true
            }
        }
    }
}