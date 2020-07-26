pipeline {
    agent any
    options {
        ansiColor('xterm')
    }
    stages {
        stage('Build') {
			agent {
				docker { image 'conanio/gcc10' }
			}
            steps {
                sh 'pwd'
                cache(caches: [[$class: 'ArbitraryFileCache', excludes: '', includes: 'package**', path: '${CONAN_CACHE_LOCATION}']], maxCacheSize: 0) {
				    sh 'conan create . --build missing --profile conan/profiles/default'
                }
				archiveArtifacts artifacts: '**', fingerprint: true
            }
        }
    }
}